/* SPDX-License-Identifier: GPL-2.0 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <inttypes.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "client-params.h"
#include "../hardware-capabilities/display-owner.h"
static volatile sig_atomic_t stop_requested;
static void signal_stop(int sig) { (void)sig; stop_requested=1; }
struct diagnose { TEEC_Context context; TEEC_Session session; struct display_owner owner;
 struct hdmi_dev_mode_request mode; uint64_t services,busy; int dirty,ddc_quarantined; };
static int now_ms(uint64_t *v) { struct timespec t;
 if(clock_gettime(CLOCK_MONOTONIC,&t)||t.tv_sec<0)return -1;
 *v=(uint64_t)t.tv_sec*1000u+(uint64_t)t.tv_nsec/1000000u;return 0; }
static int decimal(const char *s,uint32_t *v) { uint64_t n=0;if(!s||!*s)return -1;
 for(;*s;s++){if(*s<'0'||*s>'9')return -1;n=n*10u+(unsigned)(*s-'0');if(n>60000)return -1;}
 if(!n)return -1;
 *v=(uint32_t)n;return 0; }
struct diagnose_pool_plan { uint64_t stride,image,slot,need; uint32_t have,frame_base,command_bytes,pip_slot_bytes,pip_offset[2]; };
static int mode_pool_plan(const struct hdmi_dev_mode_request *m,struct diagnose_pool_plan *p)
{
 if(!m||!p)return -1;
 *p=(struct diagnose_pool_plan){.have=m->pool_bytes};
 uint32_t w=m->timing.hdisplay,h=m->timing.vdisplay;
 if(!w||w>2560u||!h||h>1440u||m->main_slots<2u||m->main_slots>3u||m->pip_enabled>1u||
 (m->pool_bytes!=HDMI_DEV_POOL_BYTES&&m->pool_bytes!=HDMI_DEV_CACHED_POOL_BYTES))return -1;
 p->stride=(uint64_t)w*3u;
 if((p->stride&15u)||p->stride>0x00ffffffu)return -1;
 p->image=p->stride*h;
 p->slot=(p->image+4095u)&~UINT64_C(4095);
 p->command_bytes=65536u;p->pip_slot_bytes=m->pip_enabled?131072u:0u;
 p->pip_offset[0]=m->pip_enabled?p->command_bytes:0u;
 p->pip_offset[1]=m->pip_enabled?p->command_bytes+p->pip_slot_bytes:0u;
 p->frame_base=p->command_bytes+2u*p->pip_slot_bytes;
 p->need=p->frame_base+p->slot*m->main_slots;
 return p->image<=UINT32_MAX&&p->slot<=UINT32_MAX&&p->need<=p->have?0:-1;
}
static int mode_valid(const struct hdmi_dev_mode_request *m) {
 TEEC_Operation op;const struct hdmi_dev_detailed_timing *t=&m->timing;
 struct diagnose_pool_plan pool;
 if(mode_pool_plan(m,&pool)){fprintf(stderr,"pool_preflight reject need=%"PRIu64" have=%u stride=%"PRIu64" slot=%"PRIu64" explicit_slots=%u pip=%u (geometry/alignment/capacity)\n",pool.need,pool.have,pool.stride,pool.slot,m->main_slots,m->pip_enabled);return 0;}
 if(hdmi_dev_init_mode_operation(&op,m)||m->main_slots<2||m->main_slots>3||m->pip_enabled>1||
 m->vpp_blanking>3||(m->caps_bits&~15u)||!t->pixel_clock_hz||!m->source_max_tmds_hz||
 !m->sink_max_tmds_hz||!t->hdisplay||t->hdisplay>2560||!t->vdisplay||t->vdisplay>1440||
 !(t->hdisplay<t->hsync_start&&t->hsync_start<t->hsync_end&&t->hsync_end<t->htotal)||
 !(t->vdisplay<t->vsync_start&&t->vsync_start<t->vsync_end&&t->vsync_end<t->vtotal)||(t->flags&~3u))return 0;
 for(unsigned i=0;i<4;i++)if(m->reserved[i])return 0;
 return 1; }
static int read_request(const char *p,struct hdmi_dev_mode_request *m) {
 FILE *f=fopen(p,"rb");if(!f)return -1;
 size_t n=fread(m,1,sizeof(*m),f);int extra=fgetc(f),bad=ferror(f);if(fclose(f))bad=1;
 return n==sizeof(*m)&&extra==EOF&&!bad&&mode_valid(m)?0:-1; }
static TEEC_Result call(struct diagnose *d,uint32_t c,TEEC_Operation *o) {
 uint32_t origin=0;TEEC_Result r=TEEC_InvokeCommand(&d->session,c,o,&origin);
 if(c!=HDMI_DEV_SERVICE||(r!=TEEC_SUCCESS&&r!=TEEC_ERROR_BUSY))
 printf("command=%"PRIu32" result=0x%08"PRIx32" origin=%"PRIu32"\n",c,r,origin);
 fflush(stdout);return r; }
_Static_assert(sizeof(struct hdmi_dev_mode_preflight)==184,"pure preflight ABI");
static TEEC_Result pure_preflight(struct diagnose *d)
{
 struct diagnose_pool_plan pool;if(mode_pool_plan(&d->mode,&pool))return TEEC_ERROR_BAD_PARAMETERS;
 struct hdmi_dev_mode_request input=d->mode;
 struct hdmi_dev_mode_preflight reply={0},expected={
  .version=HDMI_DEV_PREFLIGHT_VERSION,.size=sizeof(expected),
  .width=d->mode.timing.hdisplay,.height=d->mode.timing.vdisplay,
  .stride=(uint32_t)pool.stride,.image_bytes=(uint32_t)pool.image,
  .frame_base=pool.frame_base,.frame_slot_bytes=(uint32_t)pool.slot,
  .main_slots=d->mode.main_slots,.total_bytes=(uint32_t)pool.need,
  .pip_slot_bytes=pool.pip_slot_bytes,.pip_offset={pool.pip_offset[0],pool.pip_offset[1]},
  .command_bytes=pool.command_bytes,.flags=HDMI_DEV_PREFLIGHT_PURE,
 };
 memcpy(&expected.request,&d->mode,sizeof(d->mode));
 TEEC_Operation o={0};
 const uint32_t types=TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,TEEC_MEMREF_TEMP_OUTPUT,TEEC_NONE,TEEC_NONE);
 o.paramTypes=types;o.params[0].tmpref.buffer=&input;o.params[0].tmpref.size=sizeof(input);
 o.params[1].tmpref.buffer=&reply;o.params[1].tmpref.size=sizeof(reply);
 TEEC_Result r=call(d,HDMI_DEV_PREFLIGHT_MODE,&o);
 if(r)return r;
 if(o.paramTypes!=types||o.params[0].tmpref.buffer!=&input||o.params[1].tmpref.buffer!=&reply||
 o.params[0].tmpref.size!=sizeof(input)||o.params[1].tmpref.size!=sizeof(reply)||
 memcmp(&input,&d->mode,sizeof(input))||memcmp(&reply,&expected,sizeof(reply)))
  return TEEC_ERROR_BAD_FORMAT;
 printf("pure_preflight accepted need=%u have=%u frame_base=%u slots=%u PIP_offsets=%u,%u PIP_slot=%u\n",
 reply.total_bytes,d->mode.pool_bytes,reply.frame_base,reply.main_slots,reply.pip_offset[0],
 reply.pip_offset[1],reply.pip_slot_bytes);
 return TEEC_SUCCESS;
}
static TEEC_Result ddc_snapshot(struct diagnose *d) {
 struct hdmi_dev_ddc_snapshot p={0};TEEC_Operation o={0};
 _Static_assert(sizeof(p)==128,"DDC snapshot ABI");
 o.paramTypes=TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,TEEC_NONE,TEEC_NONE,TEEC_NONE);
 o.params[0].tmpref.buffer=&p;o.params[0].tmpref.size=sizeof(p);
 TEEC_Result e=call(d,HDMI_DEV_DDC_SNAPSHOT,&o);if(e)return e;
 if(o.params[0].tmpref.size!=sizeof(p)||p.version!=HDMI_DEV_DDC_SNAPSHOT_VERSION||
 p.size!=sizeof(p)||(p.valid_mask&~((UINT32_C(1)<<HDMI_DEV_DDC_RAW_COUNT)-1u))||
 (p.readiness&~31u)||p.flags!=7u)return TEEC_ERROR_BAD_FORMAT;
 static const uint32_t offsets[HDMI_DEV_DDC_RAW_COUNT]={
  0x1f81c,0x1f82c,0x1f830,0x1f834,0x1f838,0x1f83c,0x1f840,0x1f844,0x1f848,0x1f84c,
  0x1f814,0x1f818,0x614,0x1f850,0x1f824,0x1f810,0x414,0xc010,0x1f800,0x1f804,0x1f820,0x1f828};
 printf("DDC snapshot readiness=0x%x valid_mask=0x%x io_result=0x%08x high=%u low=%u hold=%u fast=%u\n",
 p.readiness,p.valid_mask,p.io_result,p.selected_high,p.selected_low,p.sda_hold,p.fast_mode);
 for(unsigned i=0;i<HDMI_DEV_DDC_RAW_COUNT;i++)if(p.valid_mask&(UINT32_C(1)<<i))
  printf("DDC raw offset=0x%05x value=0x%08x\n",offsets[i],p.raw[i]);
 fflush(stdout);return TEEC_SUCCESS;
}
static TEEC_Result ddc_prepare(struct diagnose *d) {
 struct hdmi_dev_ddc_prepare p={0};TEEC_Operation o={0};
 _Static_assert(sizeof(p)==80,"DDC prepare ABI");
 o.paramTypes=TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,TEEC_NONE,TEEC_NONE,TEEC_NONE);
 o.params[0].tmpref.buffer=&p;o.params[0].tmpref.size=sizeof(p);
 TEEC_Result e=call(d,HDMI_DEV_DDC_OWNER_PREPARE,&o);
 if(e||o.params[0].tmpref.size!=sizeof(p)||p.version!=HDMI_DEV_DDC_PREPARE_VERSION||
    p.size!=sizeof(p)||(p.valid_mask&~0x3ffu)||(p.flags&~15u)){
  d->ddc_quarantined=1;return e?e:TEEC_ERROR_BAD_FORMAT;
 }
 printf("DDC owner prepare result=%"PRId32" flags=0x%x writes=%u valid=0x%x epoch=%u phy=%x/%x ddc=%x\n",
  p.result,p.flags,p.writes,p.valid_mask,p.epoch,p.phy_before,p.phy_after,p.ddc_status);
 for(unsigned i=0;i<5;i++)printf("DDC owner prepare index=%u before=0x%x after=0x%x\n",
  i,p.before[i],p.after[i]);
 fflush(stdout);
 if(p.flags&2u){d->ddc_quarantined=1;return TEEC_ERROR_BUSY;}
 return TEEC_SUCCESS;
}
static TEEC_Result ddc_read(struct diagnose *d) {
 struct hdmi_dev_ddc_read p={0};TEEC_Operation o={0};
 _Static_assert(sizeof(p)==540,"DDC read ABI");
 o.paramTypes=TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,TEEC_NONE,TEEC_NONE,TEEC_NONE);
 o.params[0].tmpref.buffer=&p;o.params[0].tmpref.size=sizeof(p);
 TEEC_Result e=call(d,HDMI_DEV_DDC_READ,&o);
 if(e){d->ddc_quarantined=1;return e;}
 if(o.params[0].tmpref.size!=sizeof(p)||p.version!=HDMI_DEV_DDC_READ_VERSION||
 p.size!=sizeof(p)||p.edid_bytes>256u||p.edid_blocks>2u||p.owner_valid_mask&~31u||
 p.before.valid_mask&~0x3fffffu||p.edid_flags&~15u||p.run_flags&~7u) {
  d->ddc_quarantined=1;return TEEC_ERROR_BAD_FORMAT;
 }
 if(p.run_flags&1u)d->ddc_quarantined=1;
 printf("DDC read result=%"PRId32" readiness=%"PRId32" edid_result=%"PRId32" cleanup=%"PRId32" owner_blockers=0x%x owner_valid=0x%x recovery=%u active=%u restored=%u bytes=%u blocks=%u expected=%u edid_flags=0x%x tx=%u polls=%u elapsed_us=%"PRIu64"\n",
 p.result,p.readiness_result,p.edid_result,p.cleanup_result,p.owner_blockers,p.owner_valid_mask,
 !!(p.run_flags&1u),!!(p.run_flags&2u),!!(p.run_flags&4u),p.edid_bytes,p.edid_blocks,p.edid_expected,
 p.edid_flags,p.transactions,p.polls,((uint64_t)p.elapsed_us_high<<32)|p.elapsed_us_low);
 const uint32_t regs[5]={0x10004,0x14000,0x14004,0x1e410,0x410};
 for(unsigned i=0;i<5;i++)if(p.owner_valid_mask&(1u<<i))
  printf("DDC owner offset=0x%x value=0x%08x\n",regs[i],p.owner_raw[i]);
 printf("DDC before valid=0x%x high=%u low=%u hold=%u fast=%u last_offset=0x%x slave=0x%x segment=%u address=0x%x status=0x%x\n",
 p.before.valid_mask,p.before.selected_high,p.before.selected_low,p.before.sda_hold,p.before.fast_mode,
 p.last_offset,p.last_slave,p.last_segment,p.last_address,p.last_status);
 for(unsigned i=0;i<22;i++)if(p.before.valid_mask&(1u<<i))
  printf("DDC before index=%u raw=0x%08x\n",i,p.before.raw[i]);
 const unsigned scdc[5]={1,0x20,0x21,0x40,0x41};
 for(unsigned i=0;i<5;i++)printf("DDC SCDC address=0x%02x result=%"PRId32" value=0x%02x\n",scdc[i],p.scdc_result[i],p.scdc_value[i]);
 for(unsigned i=0;i<p.edid_blocks;i++)printf("DDC EDID block=%u checksum_sum=%u\n",i,p.checksum_sum[i]);
 for(unsigned i=0;i<p.edid_bytes;i++) {
  if(!(i%16u))printf("DDC EDID %03x:",i);
  printf(" %02x",p.edid[i]);if(i%16u==15u||i+1u==p.edid_bytes)putchar('\n');
 }
 fflush(stdout);
 if(d->ddc_quarantined) {
  fprintf(stderr,"DDC quarantine: retain session/display owner and holders; ordinary STOP is blocked\n");
  return TEEC_ERROR_BUSY;
 }
 return TEEC_SUCCESS;
}
static TEEC_Result status(struct diagnose *d,int displaying,struct hdmi_dev_status *s) {
 TEEC_Operation o;hdmi_dev_status_operation(&o,s);TEEC_Result r=call(d,HDMI_DEV_STATUS,&o);if(r)return r;
 if(!hdmi_dev_status_valid(s,o.params[0].tmpref.size))return TEEC_ERROR_BAD_FORMAT;
 printf("status state=%u geometry=%ux%u refresh_millihz=%u last_error=0x%08x flags=0x%08x service_calls=%"PRIu64" busy=%"PRIu64"\n",
 s->state,s->width,s->height,s->refresh_millihz,s->last_error,s->flags,d->services,d->busy);
 if(s->last_error||(displaying&&(s->state!=HDMI_DEV_DISPLAYING||s->width!=d->mode.timing.hdisplay||
 s->height!=d->mode.timing.vdisplay)))return TEEC_ERROR_BAD_STATE;
 return TEEC_SUCCESS; }
static TEEC_Result service(struct diagnose *d) {
 TEEC_Operation o;hdmi_dev_service_operation(&o);d->services++;TEEC_Result r=call(d,HDMI_DEV_SERVICE,&o);
 if(r==TEEC_ERROR_BUSY){d->busy++;return TEEC_SUCCESS;}return r; }
static TEEC_Result ddc_recover(struct diagnose *d) {
 struct hdmi_dev_ddc_recovery reply={0};TEEC_Operation o={0};
 _Static_assert(sizeof(reply)==48,"DDC recovery ABI");
 o.paramTypes=TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,TEEC_NONE,TEEC_NONE,TEEC_NONE);
 o.params[0].tmpref.buffer=&reply;o.params[0].tmpref.size=sizeof(reply);
 TEEC_Result result=call(d,HDMI_DEV_DDC_RECOVER,&o);
 if(result)return result;
 if(o.params[0].tmpref.size!=sizeof(reply)||reply.version!=HDMI_DEV_DDC_RECOVERY_VERSION||
    reply.size!=sizeof(reply)||(reply.flags&~31u))return TEEC_ERROR_BAD_FORMAT;
 printf("DDC recovery result=%"PRId32" original=%"PRId32" flags=0x%x status=0x%x blockers=0x%x; expired data discarded\n",
        reply.result,reply.original_result,reply.flags,reply.status,reply.owner_blockers);
 fflush(stdout);
 if(reply.result||reply.flags!=31u||reply.owner_blockers)return TEEC_ERROR_BUSY;
 d->ddc_quarantined=0;return TEEC_SUCCESS;
}
static TEEC_Result finish(struct diagnose *d) {
 if(d->ddc_quarantined)return TEEC_ERROR_BUSY;
 TEEC_Operation o={0};struct hdmi_dev_status s;TEEC_Result r=call(d,HDMI_DEV_STOP,&o);if(r)return r;
 r=status(d,0,&s);if(r)return r;if(!hdmi_dev_status_stopped(&s))return TEEC_ERROR_BAD_STATE;
 if(d->dirty&&display_owner_stopped(&d->owner))return TEEC_ERROR_GENERIC;
 d->dirty=0;TEEC_CloseSession(&d->session);display_owner_release(&d->owner);TEEC_FinalizeContext(&d->context);
 puts("STOP_STATUS_QUIESCENT confirmed; session closed; holder remains owner-managed");return TEEC_SUCCESS; }
static TEEC_Result show(struct diagnose *d,uint32_t pattern,uint32_t rgb) {
 uint64_t began,now;if(now_ms(&began))return TEEC_ERROR_GENERIC;
 while(!stop_requested) {TEEC_Operation o;if(hdmi_dev_show_operation(&o,pattern,rgb))return TEEC_ERROR_BAD_PARAMETERS;
 TEEC_Result r=call(d,HDMI_DEV_SHOW,&o);if(r!=TEEC_ERROR_BUSY)return r;
 r=service(d);if(r)return r;if(now_ms(&now)||now-began>=5000)return TEEC_ERROR_BUSY;}
 return TEEC_ERROR_CANCEL; }
static TEEC_Result wait_stage(struct diagnose *d,uint32_t ms) {
 uint64_t began,now;int input_fd=STDIN_FILENO;if(now_ms(&began))return TEEC_ERROR_GENERIC;
 puts(ms?"advance after the selected diagnostic interval":"Enter advances; ddc reads EDID/SCDC without advancing; EOF requests STOP");
 while(!stop_requested) {
 TEEC_Result r=service(d);if(r)return r;struct hdmi_dev_status s;r=status(d,1,&s);if(r)return r;
 if(now_ms(&now))return TEEC_ERROR_GENERIC;
 if(ms&&now-began>=ms)return TEEC_SUCCESS;
 struct pollfd in={.fd=input_fd,.events=POLLIN};
 int timeout=25;if(ms&&ms-(now-began)<25)timeout=(int)(ms-(now-began));
 int ready=poll(&in,1,timeout);
 if(ready<0){if(errno==EINTR)continue;return TEEC_ERROR_GENERIC;}
 if(ready&&(in.revents&(POLLIN|POLLHUP|POLLERR|POLLNVAL))) {
 char line[64];ssize_t n=read(input_fd,line,sizeof(line));
 if(n==0){if(ms){input_fd=-1;continue;}stop_requested=1;break;}if(n<0){if(errno==EINTR)continue;return TEEC_ERROR_GENERIC;}
 if(n==8&&!memcmp(line,"recover\n",8)) {
 TEEC_Result e=ddc_recover(d);if(e)return e;continue;
 }
 if(n==6&&!memcmp(line,"owner\n",6)) {
 TEEC_Result e=ddc_prepare(d);if(e)return e;continue;
 }
 if(n==4&&!memcmp(line,"ddc\n",4)) {
 TEEC_Result e=ddc_read(d);if(e)return e;continue;
 }
 if(memchr(line,10,(size_t)n))return TEEC_SUCCESS;
 }}return TEEC_ERROR_CANCEL; }
static void retained_wait(struct diagnose *d) {
 fprintf(stderr,"STOP failed: retain session/display lock and external holder; type recover to retire the owned late read, then retry to request STOP\n");
 int fd=STDIN_FILENO;for(;;) {struct pollfd in={.fd=fd,.events=POLLIN};int n=poll(&in,1,1000);
 if(n<0){if(errno==EINTR)continue;fd=-1;continue;}
 if(n){char line[64];ssize_t count=read(fd,line,sizeof(line));
 if(count<=0){if(count<0&&errno==EINTR)continue;fd=-1;continue;}
 if(count==9&&!memcmp(line,"snapshot\n",9)){ddc_snapshot(d);continue;}
 if(count==8&&!memcmp(line,"recover\n",8)){ddc_recover(d);continue;}
 if(count==6&&!memcmp(line,"retry\n",6)&&finish(d)==TEEC_SUCCESS)return;}}
}
static TEEC_Result overlay(struct diagnose *d,unsigned alpha) {
 const struct hdmi_dev_overlay p={32,32,256,128,1024};
 if(!d->mode.pip_enabled||p.x+p.width>d->mode.timing.hdisplay||p.y+p.height>d->mode.timing.vdisplay)
 return TEEC_ERROR_BAD_PARAMETERS;
 size_t bytes=(size_t)p.stride*p.height;unsigned char *rgba=malloc(bytes);if(!rgba)return TEEC_ERROR_OUT_OF_MEMORY;
 for(size_t i=0;i<bytes;i+=4){rgba[i]=255;rgba[i+1]=255;rgba[i+2]=0;rgba[i+3]=(unsigned char)alpha;}
 TEEC_Operation o={0};o.paramTypes=TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,TEEC_MEMREF_TEMP_INPUT,TEEC_NONE,TEEC_NONE);
 o.params[0].tmpref.buffer=(void *)&p;o.params[0].tmpref.size=sizeof(p);
 o.params[1].tmpref.buffer=rgba;o.params[1].tmpref.size=bytes;
 TEEC_Result r=call(d,HDMI_DEV_OVERLAY,&o);free(rgba);
 if(!r)printf("PIP request accepted: yellow, position=32,32 size=256x128 alpha=%u\n",alpha);
 return r; }
#ifndef HDMI_MODE_DIAGNOSE_TEST
int main(int argc,char **argv) {
 if(argc<2){fprintf(stderr,"usage: %s REQUEST.bin [--stage-ms 1..60000] [--pip opaque|transparent|half] [--ddc-snapshot]\n",argv[0]);return 2;}
 uint32_t ms=0;int pip=-1,snapshot_requested=0;
 for(int i=2;i<argc;i++) {
 if(!strcmp(argv[i],"--stage-ms")&&i+1<argc&&!ms){if(decimal(argv[++i],&ms))return 2;}
 else if(!strcmp(argv[i],"--pip")&&i+1<argc&&pip<0){const char *p=argv[++i];
 pip=!strcmp(p,"opaque")?255:!strcmp(p,"transparent")?0:!strcmp(p,"half")?128:-1;if(pip<0)return 2;}
 else if(!strcmp(argv[i],"--ddc-snapshot")&&!snapshot_requested)snapshot_requested=1;
 else return 2; }
 struct diagnose d={.owner=DISPLAY_OWNER_INIT};
 if(read_request(argv[1],&d.mode)||d.mode.pip_enabled!=(uint32_t)(pip>=0)){
 fprintf(stderr,"invalid112B request/pip selection mismatch\n");return 2;}
 struct diagnose_pool_plan pool;mode_pool_plan(&d.mode,&pool);
 printf("pool_preflight accepted need=%"PRIu64" have=%u explicit_slots=%u\n",pool.need,pool.have,d.mode.main_slots);
 printf("REQUEST clock_hz=%"PRIu64" h=%u/%u/%u/%u v=%u/%u/%u/%u flags=%u slots=%u pip=%u\n",
 d.mode.timing.pixel_clock_hz,d.mode.timing.hdisplay,d.mode.timing.hsync_start,d.mode.timing.hsync_end,d.mode.timing.htotal,
 d.mode.timing.vdisplay,d.mode.timing.vsync_start,d.mode.timing.vsync_end,d.mode.timing.vtotal,
 d.mode.timing.flags,d.mode.main_slots,d.mode.pip_enabled);
 struct sigaction a={.sa_handler=signal_stop};sigemptyset(&a.sa_mask);
 if(sigaction(SIGINT,&a,NULL)||sigaction(SIGTERM,&a,NULL)||sigaction(SIGHUP,&a,NULL))return 1;
 TEEC_Result r=TEEC_InitializeContext(NULL,&d.context);if(r)return 1;
 const TEEC_UUID uuid=HDMI_DEV_UUID;uint32_t origin=0;
 r=TEEC_OpenSession(&d.context,&d.session,&uuid,TEEC_LOGIN_PUBLIC,NULL,NULL,&origin);
 if(r){TEEC_FinalizeContext(&d.context);return 1;}
 r=pure_preflight(&d);
 if(r||stop_requested){fprintf(stderr,"pure preflight rejected result=0x%08"PRIx32"\n",r);
  TEEC_CloseSession(&d.session);TEEC_FinalizeContext(&d.context);return r?2:1;}
 /* TX INIT 在显示锁内检查自己的队列。 */
 if(display_owner_acquire(&d.owner,0)){TEEC_CloseSession(&d.session);TEEC_FinalizeContext(&d.context);return 1;}
 d.dirty=1;if(display_owner_begin(&d.owner,1)){retained_wait(&d);return 1;}
 TEEC_Operation o;hdmi_dev_init_mode_operation(&o,&d.mode);r=call(&d,HDMI_DEV_INIT_MODE,&o);
 if(!r&&snapshot_requested)r=ddc_snapshot(&d);
 if(!r&&pip>=0)r=overlay(&d,(unsigned)pip);
 const uint32_t patterns[]={HDMI_DEV_PATTERN_SOLID,HDMI_DEV_PATTERN_SOLID,HDMI_DEV_PATTERN_COORDINATE};
 const uint32_t rgb[]={0xff0000,0x00ffff,0};const char *names[]={"red","cyan","coordinate"};
 for(unsigned i=0;i<3&&!r&&!stop_requested;i++){
 r=show(&d,patterns[i],rgb[i]);struct hdmi_dev_status s;if(!r)r=status(&d,1,&s);
 if(!r){printf("stage request accepted: %s\n",names[i]);fflush(stdout);r=wait_stage(&d,ms);}
 }
 if(finish(&d))retained_wait(&d);
 return r==TEEC_SUCCESS&&!stop_requested?0:1;
}
#endif

#include "bcm_drv_hash.h"
#include "bcm_drv_spad.h"
#include "bcm_drv_xbar.h"
#include "bcm_drv_dma.h"
#include "bcm_drv_hash.h"

void bcm_drv_hash_reset(void)
{
    volatile BCM_HASH *hash = (BCM_HASH *) BASE_HASH;
    hash->hash_control=0x4;
    hash->hash_control=0x0;
}

static void _drv_hash_init(uint32_t  hash_mode)
{
    volatile struct hash * pHASH = (volatile struct hash *) BASE_HASH; 
    volatile BCM_HASH *hash = (BCM_HASH *) BASE_HASH;
    struct hash_Hash_Configure cfg;
    uint32_t alg_select;
    switch(hash_mode)
    {
        case HASH_MODE_SHA1  : alg_select=0x0; break;
        case HASH_MODE_SHA256: alg_select=0x1; break;
        case HASH_MODE_SHA224: alg_select=0x2; break;
        case HASH_MODE_MD5   : alg_select=0x3; break;
        case HASH_MODE_SHA512: alg_select=0x4; break;
        case HASH_MODE_SHA384: alg_select=0x5; break;
        default:               alg_select=0x6; //AVOID this: Warning: C3017W: alg_select may be used before being set
    }

    cfg.u.value = pHASH->Hash_Configure.u.value;
    cfg.u.b.byte_swap        = 1 ; //0=big endian, 1=little endian [reset value]
    cfg.u.b.output_data_mode = 0;
    cfg.u.b.hash_mode        = 0;  //hash
    cfg.u.b.alg_select       = alg_select;
    pHASH->Hash_Configure.u.value = cfg.u.value;

    hash->total_message_size_low_word  = 0;
    hash->total_message_size_high_word = 0;
    hash->hash_control=0x1;
    hash->hash_command=0x1;
}

static void _drv_hash_update(uint32_t size_byte)
{
    volatile BCM_HASH *hash = (BCM_HASH *) BASE_HASH;
    uint64_t hashMessageLenByte;
    hashMessageLenByte = hash->total_message_size_high_word;
    hashMessageLenByte = hashMessageLenByte<<32;
    hashMessageLenByte|= (hash->total_message_size_low_word);
    hashMessageLenByte+=size_byte;
    hash->total_message_size_low_word  = hashMessageLenByte;
    hash->total_message_size_high_word = hashMessageLenByte>>32;
    hash->incoming_message_segment_size_word = size_byte;

    hash->hash_control=0x2;
    hash->hash_command=0x1;
}


static void _drv_hash_final(uint32_t size_byte, uint32_t hwPad)
{
    volatile BCM_HASH *hash = (BCM_HASH *) BASE_HASH; 
    uint64_t hashMessageLenByte;
    uint32_t temp=(hwPad)?0x4:0;   
    hash->incoming_message_segment_size_word = size_byte;

    hashMessageLenByte = hash->total_message_size_high_word;
    hashMessageLenByte = hashMessageLenByte<<32;
    hashMessageLenByte|= (hash->total_message_size_low_word);
    hashMessageLenByte+=size_byte;
    hash->total_message_size_low_word  = hashMessageLenByte;
    hash->total_message_size_high_word = hashMessageLenByte>>32;
    hash->hash_control=0x3|temp;
    hash->hash_command=0x1;
}

static void _drv_hash_get_digest(uint32_t* digest, uint32_t* digest_size_byte)
{
    volatile BCM_HASH *hash = (BCM_HASH *) BASE_HASH;
    volatile uint32_t *digest_pointer_lo = &hash->hash_digest_and_context_a;
    volatile uint32_t *digest_pointer_hi = &hash->hash_digest_and_context_a_high_word;
    uint32_t i,j,digest_len_words=0;
    uint32_t hash_alg = (hash->hash_configuration)&0x7;

    switch(hash_alg)
    {
        case 0x0: digest_len_words=5; break; //SHA1
        case 0x1: digest_len_words=8; break; //SHA256
        case 0x2: digest_len_words=7; break; //SHA224
        case 0x3: digest_len_words=4; break; //MD5
        case 0x4: digest_len_words=16;break; //SHA512
        case 0x5: digest_len_words=12;break; //SHA384
        default:
			EMSG("Not support hash alg: 0x%x.\n", hash_alg);
        	return;
    }

    if(digest_len_words==12||digest_len_words==16)
    {
        // digest has to be re-interleaved
        for(i=j=0; j< digest_len_words/2; i+=2, j+=1)
        {
             digest[i]   = *(digest_pointer_hi+j);
             digest[i+1] = *(digest_pointer_lo+j);
        }
    }
    else
    {
        for(i=0;i<digest_len_words;i++)
            digest[i]=*(digest_pointer_lo+i);
    }

     *digest_size_byte = digest_len_words*sizeof(uint32_t);
}


static void _drv_hmac_init(uint32_t hash_mode, uint8_t* key, uint32_t size_byte)
{
    volatile BCM_HASH *hash = (BCM_HASH *) BASE_HASH;
    volatile struct hash * pHASH = (volatile struct hash *) BASE_HASH; 
    struct hash_Hash_Configure cfg;
    uint32_t alg_select;
    uint32_t i,size_words;
  
    switch(hash_mode)
    {
        case HASH_MODE_SHA1  : alg_select=0x0; break;
        case HASH_MODE_SHA256: alg_select=0x1; break;
        case HASH_MODE_SHA224: alg_select=0x2; break;
        case HASH_MODE_MD5   : alg_select=0x3; break;
        case HASH_MODE_SHA512: alg_select=0x4; break;
        case HASH_MODE_SHA384: alg_select=0x5; break;
        default:               alg_select=0x6; //AVOID this: Warning: C3017W: alg_select may be used before being set
    }
  
    cfg.u.value = pHASH->Hash_Configure.u.value;
    cfg.u.b.byte_swap        = 1; //0=big endian, 1=little endian [reset value]
    cfg.u.b.output_data_mode = 0;
    cfg.u.b.hash_mode        = 1; //hmac
    cfg.u.b.alg_select       = alg_select;
    pHASH->Hash_Configure.u.value = cfg.u.value;
  
    hash->hmac_key_length = size_byte;
    
    // load key bits
    size_words=size_byte/4;
    if((size_byte&0x3)!=0x0) size_words+=1;
  
    for(i=0; i< size_words; i++)
    {
        *(&hash-> hmac_key_0 + i) = *((uint32_t*) key+i);
    }
  
    hash->total_message_size_low_word  = 0;
    hash->total_message_size_high_word = 0;
    hash->hash_control=0x1;
    hash->hash_command=0x1;
  
}

STATUS  bcm_drv_hash_wait_4_done(uint32_t time_out)
{
    volatile BCM_APB_ADEC *apb_adec= (BCM_APB_ADEC *) BASE_APB_ADEC;
    volatile BCM_HASH *hash = (BCM_HASH *) BASE_HASH;
    volatile uint32_t temp = 0;
    while(time_out--)
    {
        temp=hash->hash_status;

        //clear and check interrupt
        if(temp&0x1)
        {
            //clear
            hash->hash_status=temp;
            apb_adec->address_decoder_interrupt_register=0x100;//ENG8

            //check error bits
            if(temp&0xe)
            {
                bcm_drv_hash_reset();
                return (STATUS_FAILURE);
            }
            else
                return (STATUS_SUCCESS);
        }
    }
    return (STATUS_HASH_TIMEOUT);
}


STATUS bcm_drv_hash_init(uint32_t hash_mode)
{
    _drv_hash_init(hash_mode);

    return bcm_drv_hash_wait_4_done(1000000);
}

STATUS bcm_drv_hash_update(uint8_t* msg, uint32_t msg_size_byte)
{
    uint32_t round     = 0;
    uint32_t residual  = 0;
    uint32_t proc_size = 0;
    uint32_t i         = 0;
    uint32_t status    = STATUS_FAILURE;
    uint32_t offset    = 0;


    if((msg_size_byte%256)!=0)
        return status;

    residual = msg_size_byte%BCM_V3_SPAD_SIZE_IN_BYTE;
    round    = msg_size_byte/BCM_V3_SPAD_SIZE_IN_BYTE;

    round = (residual!=0)?(round+1):round;
    proc_size = residual;

    for(i=0;i<round;i++)
    {
        bcm_drv_spad_set_access_mode(SPAD_CPU_ACCESS);
        bcm_drv_spad_set_wr_ptr(0);
        bcm_drv_spad_set_rd_ptr(0);

        bcm_drv_spad_write((uint32_t*)(msg + offset), proc_size/sizeof(uint32_t), 0);

        bcm_drv_xbar_set_path(AES_HASH_PATH);
        bcm_drv_spad_set_access_mode(SPAD_SEQUENTIAL_ACCESS);
        bcm_drv_spad_set_wr_ptr(proc_size/sizeof(uint32_t));
        bcm_drv_spad_set_rd_ptr(0);

        _drv_hash_update(proc_size);

        status = bcm_drv_hash_wait_4_done(1000000);
        if(status) return status;

        offset   +=proc_size;
        proc_size =BCM_V3_SPAD_SIZE_IN_BYTE;
    }

    return status;
}

STATUS bcm_drv_hash_update_ext(uint32_t soc_msg_addr, uint32_t msg_size_byte)
{
    STATUS status = STATUS_FAILURE;
    uint32_t ref_size = 0;

    bcm_drv_xbar_set_path(HASH_AES_PATH);

    bcm_drv_dma_setup_link_list(0x0, soc_msg_addr, (msg_size_byte+3)/sizeof(uint32_t), 0, &ref_size);
    security_assert(ref_size == (msg_size_byte+3)/sizeof(uint32_t));

    bcm_drv_dma_enable(0x0);
    _drv_hash_update(msg_size_byte);

    status = bcm_drv_dma_wait_4_done(0x0, 10000000);
    if(status) return status;

    return bcm_drv_hash_wait_4_done(1000000);

}

STATUS bcm_drv_hash_final(uint8_t* msg, uint32_t msg_size_byte, uint8_t* digest, uint32_t*digest_size_byte)
{
    register uint32_t round     = 0;
    register uint32_t residual  = 0;
    uint32_t proc_size = 0;
    uint32_t i         = 0;
    uint32_t status    = STATUS_FAILURE;
    uint32_t offset    = 0;

    residual = msg_size_byte%BCM_V3_SPAD_SIZE_IN_BYTE;
    round    = msg_size_byte/BCM_V3_SPAD_SIZE_IN_BYTE;

    proc_size = BCM_V3_SPAD_SIZE_IN_BYTE;

    for(i=0;i<round;i++)
    {
        bcm_drv_spad_set_access_mode(SPAD_CPU_ACCESS);
        bcm_drv_spad_set_wr_ptr(0);
        bcm_drv_spad_set_rd_ptr(0);

        bcm_drv_spad_write((uint32_t*)(msg + offset), proc_size/sizeof(uint32_t), 0);

        bcm_drv_xbar_set_path(AES_HASH_PATH);
        bcm_drv_spad_set_access_mode(SPAD_SEQUENTIAL_ACCESS);
        bcm_drv_spad_set_wr_ptr(proc_size/sizeof(uint32_t));
        bcm_drv_spad_set_rd_ptr(0);

        _drv_hash_update(proc_size);

        status = bcm_drv_hash_wait_4_done(1000000);

        if(status != STATUS_SUCCESS) return status;

        offset   += proc_size;
        proc_size = BCM_V3_SPAD_SIZE_IN_BYTE;
    }

    if(residual != 0)
    {
        bcm_drv_spad_set_access_mode(SPAD_CPU_ACCESS);
        bcm_drv_spad_set_wr_ptr(0);
        bcm_drv_spad_set_rd_ptr(0);

        bcm_drv_spad_write((uint32_t*)(msg + offset), (residual + 3)/sizeof(uint32_t), 0);

        bcm_drv_xbar_set_path(AES_HASH_PATH);
        bcm_drv_spad_set_access_mode(SPAD_SEQUENTIAL_ACCESS);
        bcm_drv_spad_set_wr_ptr((residual + 3)/sizeof(uint32_t));
        bcm_drv_spad_set_rd_ptr(0);
    }

    _drv_hash_final(residual, 1);

    status = bcm_drv_hash_wait_4_done(1000000);

    if(status != STATUS_SUCCESS) return status;

    _drv_hash_get_digest((uint32_t*)digest, digest_size_byte);

    return status;
}


STATUS bcm_drv_hash_final_ext(uint32_t soc_msg_addr, uint32_t msg_size_byte, uint8_t* digest, uint32_t*digest_size_byte)
{
    STATUS status = STATUS_FAILURE;
    uint32_t ref_size = 0;

    bcm_drv_xbar_set_path(HASH_AES_PATH);

    bcm_drv_dma_setup_link_list(0x0, soc_msg_addr, (msg_size_byte+3)/sizeof(uint32_t), 0, &ref_size);
    security_assert(ref_size == (msg_size_byte+3)/sizeof(uint32_t));

    bcm_drv_dma_enable(0x0);
    _drv_hash_final(msg_size_byte, 1);

    status = bcm_drv_dma_wait_4_done(0x0, 10000000);
    if(status) return status;

    status = bcm_drv_hash_wait_4_done(1000000);
    if(status) return status;

    _drv_hash_get_digest((uint32_t*)digest, digest_size_byte);

    return STATUS_SUCCESS;
}

STATUS bcm_drv_hmac_init(uint32_t hash_mode, uint8_t* key, uint32_t key_size)
{
    _drv_hmac_init(hash_mode, key, key_size);

    return bcm_drv_hash_wait_4_done(10000000);
}


STATUS bcm_drv_hmac_update(uint8_t* msg, uint32_t size_byte)
{
    return bcm_drv_hash_update(msg, size_byte);
}


STATUS bcm_drv_hmac_update_ext(uint32_t soc_msg_addr, uint32_t size_byte)
{
    return bcm_drv_hash_update_ext(soc_msg_addr, size_byte);
}

STATUS bcm_drv_hmac_final(uint8_t* msg, uint32_t size_byte, uint8_t* digest, uint32_t* digest_size)
{
    return bcm_drv_hash_final(msg, size_byte, digest, digest_size);
}

STATUS bcm_drv_hmac_final_ext(uint32_t soc_msg_addr, uint32_t size_byte, uint8_t* digest, uint32_t* digest_size)
{
    return bcm_drv_hash_final_ext(soc_msg_addr, size_byte, digest, digest_size);
}

void bcm_drv_hmac_get_digest(uint8_t* digest, uint32_t* digest_size_byte)
{
    return _drv_hash_get_digest((uint32_t*)digest, digest_size_byte);
}


STATUS bcm_drv_hash(uint32_t hash_mode,uint8_t* msg, uint32_t msg_size_byte, uint32_t* digest, uint32_t* digest_size_byte)
{
    STATUS status = STATUS_FAILURE;

    bcm_drv_hash_reset();

    _drv_hash_init(hash_mode);

    status = bcm_drv_hash_wait_4_done(1000000);

    if(status != STATUS_SUCCESS) return status;

    status = bcm_drv_hash_final(msg, msg_size_byte, (uint8_t*)digest, digest_size_byte);

    return status;
}


STATUS bcm_drv_hash_ext(uint32_t hash_mode,uint32_t soc_msg_addr,
        uint32_t msg_size_byte, uint32_t* digest, uint32_t* digest_size_byte)
{
    STATUS status = STATUS_FAILURE;
    uint32_t ref_size = 0;

    bcm_drv_hash_reset();

    _drv_hash_init(hash_mode);

    status = bcm_drv_hash_wait_4_done(1000000);
    if(status != STATUS_SUCCESS) return status;

    bcm_drv_xbar_set_path(HASH_AES_PATH);

    bcm_drv_dma_setup_link_list(0x0, soc_msg_addr, (msg_size_byte+3)/sizeof(uint32_t), 0, &ref_size);
    security_assert(ref_size == (msg_size_byte+3)/sizeof(uint32_t));
    
    bcm_drv_dma_enable(0x0);
    _drv_hash_final(msg_size_byte, 1);

    status = bcm_drv_dma_wait_4_done(0x0, 10000000);
    if(status) return status;

    status = bcm_drv_hash_wait_4_done(1000000);
    if(status) return status;

    _drv_hash_get_digest(digest, digest_size_byte);

    return STATUS_SUCCESS;
}



STATUS bcm_drv_hmac(uint32_t hash_mode,
                       uint8_t* msg,
                       uint32_t msg_size_byte,
                       uint8_t* key,
                       uint32_t key_size,
                       uint32_t* digest, 
                       uint32_t* digest_size_byte)
{
    STATUS status = STATUS_FAILURE;

    bcm_drv_hash_reset();

    status = bcm_drv_hmac_init(hash_mode, key, key_size);

    if(status != STATUS_SUCCESS) return status;

    return bcm_drv_hmac_final(msg, msg_size_byte, (uint8_t*)digest, digest_size_byte);
}


STATUS bcm_drv_hmac_ext(uint32_t hash_mode,
                       uint32_t soc_msg_addr,
                       uint32_t msg_size_byte,
                       uint8_t* key,
                       uint32_t key_size,
                       uint32_t* digest,
                       uint32_t* digest_size_byte)
{
    STATUS status = STATUS_FAILURE;
    uint32_t ref_size = 0;

    bcm_drv_hash_reset();

    status = bcm_drv_hmac_init(hash_mode, key, key_size);
    if(status != STATUS_SUCCESS) return status;

    status = bcm_drv_hash_wait_4_done(1000000);
    if(status != STATUS_SUCCESS) return status;

    bcm_drv_xbar_set_path(HASH_AES_PATH);

    bcm_drv_dma_setup_link_list(0x0, soc_msg_addr, (msg_size_byte+3)/sizeof(uint32_t), 0, &ref_size);
    security_assert(ref_size == (msg_size_byte+3)/sizeof(uint32_t));

    bcm_drv_dma_enable(0x0);
    _drv_hash_final(msg_size_byte, 1);

    status = bcm_drv_dma_wait_4_done(0x0, 10000000);
    if(status) return status;

    status = bcm_drv_hash_wait_4_done(1000000);
    if(status) return status;

    bcm_drv_hmac_get_digest((uint8_t*)digest, digest_size_byte);

    return STATUS_SUCCESS;
}

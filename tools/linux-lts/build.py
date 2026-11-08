#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-only
"""构建 VS680 Linux 6.12 和 OP-TEE 设备树。"""
import argparse
import json
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[2]
SRC = ROOT / 'sources/astra-current/linux_6_12-main'
HERE = Path(__file__).resolve().parent


def run(args, **kwargs):
    return subprocess.run([str(x) for x in args], check=True, **kwargs)


def board_dts(tee='optee'):
    return (ROOT / 'boards/vs680.dts').read_text()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--tee', choices=['optee'], default='optee')
    parser.add_argument('--configure-only', action='store_true')
    args = parser.parse_args()
    variant = 'linux-lts-6.12' if args.tee == 'vendor' else 'linux-optee-6.12'
    build = ROOT / 'build' / variant
    out = ROOT / 'out' / variant
    build.mkdir(parents=True, exist_ok=True)
    out.mkdir(parents=True, exist_ok=True)
    sources = json.loads((ROOT / 'sources.lock.json').read_text())
    commit_time = sources['linux_6_12-main']['timestamp']
    make = ['make', '-C', SRC, f'O={build}', 'ARCH=arm64',
            'CROSS_COMPILE=aarch64-linux-gnu-', 'LOCALVERSION=',
            'KBUILD_BUILD_USER=vs680', 'KBUILD_BUILD_HOST=build',
            'KBUILD_BUILD_VERSION=1', f'KBUILD_BUILD_TIMESTAMP={commit_time}']
    run(make + ['dolphin_core_defconfig'])
    config = SRC / 'scripts/config'
    enabled = '''SYSVIPC POSIX_MQUEUE NAMESPACES UTS_NS IPC_NS USER_NS PID_NS NET_NS
        CGROUPS CGROUP_SCHED FAIR_GROUP_SCHED CFS_BANDWIDTH CGROUP_PIDS CGROUP_CPUACCT
        CPUSETS MEMCG CGROUP_DEVICE CGROUP_FREEZER BLK_CGROUP CGROUP_BPF BPF BPF_SYSCALL
        BPF_JIT SECCOMP SECCOMP_FILTER KEYS CHECKPOINT_RESTORE
        NET INET IPV6 PACKET UNIX INET_DIAG NETLINK_DIAG NETFILTER NETFILTER_ADVANCED
        NF_CONNTRACK NF_NAT NF_TABLES NF_TABLES_INET NF_TABLES_IPV4 NF_TABLES_IPV6
        NF_TABLES_BRIDGE NFT_CT NFT_NAT NFT_MASQ NFT_REDIR NFT_REJECT NFT_LIMIT
        NFT_LOG NFT_FIB_INET NFT_FIB_IPV4 NFT_FIB_IPV6 NFT_COMPAT
        NETFILTER_XTABLES NETFILTER_XT_TARGET_MASQUERADE NETFILTER_XT_MATCH_ADDRTYPE
        NETFILTER_XT_MATCH_CONNTRACK NETFILTER_XT_MATCH_COMMENT NETFILTER_XT_MATCH_MULTIPORT
        NETFILTER_XT_MATCH_MARK NETFILTER_XT_TARGET_MARK NETFILTER_XT_MATCH_STATE
        IP_NF_IPTABLES IP_NF_FILTER IP_NF_NAT IP_NF_MANGLE IP_NF_RAW
        IP6_NF_IPTABLES IP6_NF_FILTER IP6_NF_NAT VETH BRIDGE BRIDGE_NETFILTER
        TUN VLAN_8021Q MACVLAN IPVLAN VXLAN NET_SCH_FQ NET_SCH_FQ_CODEL
        OVERLAY_FS EXT4_FS EXT4_FS_POSIX_ACL EXT4_FS_SECURITY TMPFS TMPFS_POSIX_ACL
        TMPFS_XATTR DEVTMPFS DEVTMPFS_MOUNT FHANDLE EPOLL INOTIFY_USER FANOTIFY
        AUTOFS_FS PROC_FS SYSFS SECURITY SECURITYFS SECURITY_YAMA
        SECURITY_APPARMOR DEFAULT_SECURITY_APPARMOR AUDIT AIO IO_URING SWAP COMPACTION ZSMALLOC
        ARM64_PAN UNMAP_KERNEL_AT_EL0 CMDLINE_FORCE VIRTIO VIRTIO_MMIO VIRTIO_PCI VIRTUALIZATION KVM
        HW_RANDOM HW_RANDOM_VIRTIO RANDOM_TRUST_CPU IKCONFIG IKCONFIG_PROC KALLSYMS
        PRINTK_TIME MAGIC_SYSRQ BINFMT_ELF COMPAT_BINFMT_ELF BINFMT_SCRIPT
        BLK_DEV_INITRD RD_GZIP RD_XZ RD_LZ4 WATCHDOG DW_WATCHDOG
        CPU_FREQ_DEFAULT_GOV_SCHEDUTIL CPU_FREQ_GOV_PERFORMANCE CPU_FREQ_GOV_ONDEMAND
        USB_XHCI_HCD USB_XHCI_PLATFORM USB_DWC3 USB_DWC3_HOST USB_STORAGE USB_UAS
        USB_HID HID_GENERIC HIDRAW USB_GADGET USB_CONFIGFS USB_CONFIGFS_F_HID USB_CONFIGFS_F_FS
        USB_USBNET USB_NET_CDCETHER USB_RTL8152
        MMC MMC_BLOCK MMC_SDHCI MMC_SDHCI_PLTFM MMC_SDHCI_OF_DWCMSHC
        STMMAC_ETH STMMAC_PLATFORM DWMAC_GENERIC REALTEK_PHY
        THERMAL THERMAL_OF SENSORS_DOLPHIN REGULATOR_SY8827N REGULATOR_MP886X
        WLAN WLAN_VENDOR_REALTEK RFKILL FW_LOADER_COMPRESS
        FW_LOADER_COMPRESS_XZ FW_LOADER_COMPRESS_ZSTD MEDIA_SUPPORT
        MEDIA_CAMERA_SUPPORT MEDIA_USB_SUPPORT SOUND USB_SERIAL'''.split()
    disabled = '''OPTEE HW_RANDOM_OPTEE BERLIN_TZD BERLIN_SHM BERLIN_VPP_MEM
        BERLIN_VPP BERLIN_CC BERLIN_MSGQ BERLIN_VPU SYNA_ISP BERLIN_TSP
        BERLIN_AXI_METER BERLIN_AOUT BERLIN_NSK BERLIN_OVP BERLIN_OVP_V4L2
        BERLIN_CEC HIFI_DSP BERLIN_VSYNC BERLIN_HRX BERLIN_IFCP BERLIN_BM
        SYNA_DRM SYNA_MC_DFC MEDIA_DRM SYNA_PLATFORM_CEC SYNAPTICS_HRX_V4L2
        POWERVR_ROGUE POWERVR_ROGUE_WS SYNAPTICS_VIDEO SYNAPTICS_SOUND
        DRM FB BACKLIGHT_CLASS_DEVICE
        BT_RFKILL BT_LPM BCMDHD BCMDHD103 BCMDHD591
        CPU_FREQ_DEFAULT_GOV_USERSPACE CPU_FREQ_DEFAULT_GOV_PERFORMANCE
        DEBUG_INFO DEBUG_INFO_DWARF_TOOLCHAIN_DEFAULT DEBUG_INFO_BTF
        MODULE_SIG_FORCE PANIC_ON_OOPS LOCALVERSION_AUTO DEFAULT_SECURITY_DAC
        CMDLINE_FROM_BOOTLOADER'''.split()
    if args.tee == 'optee':
        disabled.remove('OPTEE')
        disabled.remove('CMDLINE_FROM_BOOTLOADER')
        enabled += ['TEE', 'OPTEE', 'CMDLINE_FROM_BOOTLOADER', 'WATCHDOG_SYSFS',
                    'DMABUF_HEAPS_EXTRA', 'DMABUF_HEAPS_CMA_CUST',
                    'DMABUF_HEAPS_SYSTEM_CUST', 'DMABUF_HEAPS_BERLIN']
        disabled.append('CMDLINE_FORCE')
    command = [config, '--file', build / '.config']
    for name in enabled:
        command += ['--enable', name]
    for name in ['EXFAT_FS', 'XFS_FS', 'BTRFS_FS', 'NFS_FS', 'NFSD',
                 'FUSE_FS', 'SQUASHFS', 'BINFMT_MISC', 'VIRTIO_BLK', 'VIRTIO_NET',
                 'CFG80211', 'MAC80211', 'RTW89', 'RTW89_8852BE', 'VIDEO_DEV',
                 'USB_VIDEO_CLASS', 'SND', 'SND_USB_AUDIO', 'USB_ACM',
                 'USB_SERIAL_CH341', 'USB_SERIAL_CP210X', 'USB_NET_CDC_NCM']:
        command += ['--module', name]
    for name in disabled:
        command += ['--disable', name]
    command += ['--set-str', 'LOCALVERSION', '',
                '--set-str', 'CMDLINE', 'console=ttyS0,115200 '
                'earlycon=uart8250,mmio32,0xf7fcd000 root=/dev/mmcblk0p16 '
                'rootfstype=ext4 rootwait ro init=/init net.ifnames=0 cma=16M',
                '--set-str', 'LSM', 'landlock,lockdown,yama,loadpin,safesetid,ipe,apparmor,bpf',
                '--set-val', 'PANIC_TIMEOUT', '10']
    run(command)
    run(make + ['olddefconfig'])
    configured = (build / '.config').read_text()
    for name in ['USB_CONFIGFS_F_HID', 'USB_F_HID', 'USB_CONFIGFS_F_FS', 'USB_FUNCTIONFS', 'HIDRAW', 'USB_DWC2_DUAL_ROLE', 'DW_WATCHDOG']:
        if f'CONFIG_{name}=y\n' not in configured:
            raise RuntimeError(f'缺少内核配置：{name}')
    (out / 'board.dts').write_text(board_dts(args.tee))
    if args.configure_only:
        return
    run(make + [f'-j{min(os.cpu_count() or 4, 12)}', 'Image', 'modules', 'scripts_dtc'])
    run([build / 'scripts/dtc/dtc', '-I', 'dts', '-O', 'dtb', '-o',
         out / 'board.dtb', out / 'board.dts'])
    run(make + [f'INSTALL_MOD_PATH={out / "modules"}', 'modules_install'])
    for source, name in [(build / 'arch/arm64/boot/Image', 'Image'),
                         (build / '.config', 'kernel.config'),
                         (build / 'System.map', 'System.map')]:
        (out / name).write_bytes(source.read_bytes())
    metadata = {'kernel_commit': sources['linux_6_12-main']['revision'],
                'drivers_commit': sources['linux_6_12-drivers-synaptics']['revision'],
                'tee': args.tee, 'hid_fast_poll': True,
                'compiler': run(['aarch64-linux-gnu-gcc', '--version'],
                                capture_output=True, text=True).stdout.splitlines()[0]}
    (out / 'sources.json').write_text(json.dumps(metadata, indent=2) + '\n')
    print(out)


if __name__ == '__main__':
    main()

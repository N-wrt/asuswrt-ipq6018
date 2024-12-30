支持360V6 AX18 AX5 W212X均可开机能联网，但还有大量bug

注意：
=
1. **不**要用 **root** 用户 git 和编译！！！
2. 国内用户编译前最好准备好梯子

## 未完成项
1.固件内升级，目前固件不支持华硕版本的uboot使用的op的uboot
2.uboot升级掉配置，为提高固件兼容性，不修改内核强行修改固件分区，使用了和华硕官方一样的将配置分区放在了ubi分区内，而现有的op的uboot升级为刷写整个ubi分区，会导致清除ubi分区中的nvram
3.fayctory分区修补，需要将mac地址等信息写入到ubi中的faycoty中，如机型、mac地址、地区等
4.aimesh，补足上一项内容就有可能能够驱动aimesh
5.软件中心暂未移植，计划固件本体功能完善后再移植
6.其他ipq6k机器适配，目前手上只有已适配的这四个型号的机器，均为群友赞助，可自行维护提交代码或提供机器

## 编译

1. 首先装好 Ubuntu 64bit，推荐  Ubuntu  18 LTS x64 /  Mint 19.1

sudo dpkg --add-architecture i386

2. 命令行输入 `sudo apt-get update` ，然后输入
`
sudo apt-get -y install build-essential asciidoc binutils bzip2 gawk gettext git libncurses5-dev libz-dev patch python3.5 python2.7 unzip zlib1g-dev lib32gcc1 libc6-dev-i386 subversion flex uglifyjs git-core gcc-multilib p7zip p7zip-full msmtp libssl-dev texinfo libglib2.0-dev xmlto qemu-utils upx libelf-dev autoconf automake libtool autopoint device-tree-compiler g++-multilib antlr3 gperf wget libncurses5:i386 libelf1:i386 lib32z1 lib32stdc++6 gtk-doc-tools intltool binutils-dev cmake lzma liblzma-dev lzma-dev uuid-dev liblzo2-dev xsltproc dos2unix libstdc++5 docbook-xsl-* sharutils autogen shtool gengetopt libltdl-dev libtool-bin libglib2.0-dev-bin
`

3. 使用 `git clone https://github.com/stkuroneko/asuswrt-ipq6018.git` 命令下载好源代码

4. 使用 `git clone https://github.com/SWRT-dev/qca-toolchains` 命令下载toolchains

5. `mkdir asuswrt-ipq6018-build`

6. `rsync -a --del asuswrt-ipq6018/ asuswrt-ipq6018-build`

7. 分别执行 `cd qca-toolchains`

    `sudo ln -sf $(pwd)/openwrt-gcc520_musl.arm /opt/`

8. 然后 `cd ../asuswrt-ipq6018-build/release/src-qca-cypress` 进入目录

9. 输入 `make rt-360v6` 或 `make rt-ax18` 或 `make rt-ax5` 即可开始编译你要的固件了。

10. 编译完成后输出固件路径：release/src-qca-cypress/image



假如你对OpenWrt开发感兴趣，黑猫强烈推荐佐大的OpenWrt培训班，报名地址：https://forgotfun.org/2018/04/openwrt-training-2018.html

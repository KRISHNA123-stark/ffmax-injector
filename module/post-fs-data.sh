#!/system/bin/sh
MODDIR=${0%/*}
chmod 755 $MODDIR/zygisk/*.so 2>/dev/null

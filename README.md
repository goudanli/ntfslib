# ntfslib
ntfs解析 该库移植于ntfs-3g，但有所改变，目的是为了解析raw，qcow2，vmdk，vhd等虚拟磁盘，从而实现对虚拟磁盘中的ntfs文件系统的元数据分析。
使用该库只需要提供相应的磁盘读写接口，与该库一起编译自己的ntfs操作程序即可


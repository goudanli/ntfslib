# ntfslib
该程序对ntfslib的测试,磁盘对象是整个磁盘（不是磁盘分区），参数的偏移是某个分区相对于整个磁盘的偏移，所以在这之前我们需要编写代码获取磁盘分区，分区偏移，分区类型，如果分区类型是NTFS，我们就可以使用该库对分区中的ntfs文件系统的元数据进行分析

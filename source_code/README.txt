将city.txt, ds_output.txt, passenger.txt, timetable.txt这四个文本文件放在D盘下
city.txt 中存储这城市以及城市的风险值
passenger.txt 中存储着乘客的信息（以文件输入的方式添加乘客）既可以通过文件输入乘客  也可以通过程序的窗口输入乘客
格式为 乘客编号 起点 终点 出发日期 出发时间 策略（>0表示限时最小风险策略 -1表示不限时最小风险策略 -2表示最短时间到达策略）例如01 黑龙江 广东 0 4 -1
timetable.txt 中存储着时刻表
ds_output.txt 是日志文件
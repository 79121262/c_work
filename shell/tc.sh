abc=`python <<EOF
print(1+2)
print(2)
num=1
if num==1:
  print("hello")
EOF`

echo "------"
echo $abc
echo "------"
your_name="qinjx"
echo $your_name

for skill in 'Ada' 'Coffe' 'Action' 'Java'; do
    echo "I am good at ${skill}Script"
done

echo "执行的文件名：$0";
echo "第一个参数为：$1";

funWithReturn(){
    echo "这个函数会对输入的两个数字进行相加运算..."
    echo "输入第一个数字: "
    #read aNum
    aNum="hehe";
    echo "输入第二个数字: "
    anotherNum=123
    echo "两个数字分别为 $aNum 和 $anotherNum !"
    return $(($anotherNum))
}
funWithReturn
echo "输入的两个数字之和为 $? !"
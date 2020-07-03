//力扣 面试题16.01 交换数字 觉得挺好玩
//题目描述 编写一个函数，不用临时变量，直接交换numbers = [a, b]中a与b的值。
//输入: numbers = [1,2]
//输出: [2,1]
//思想：采用异或对数组元素进行操作 思想在于异或自己得0，而0异或0得0，1异或0得1
vector<int> swapNumbers(vector<int>& numbers) {
        numbers[0]^=numbers[1];
        numbers[1]^=numbers[0];
        numbers[0]^=numbers[1];
        return numbers;
}

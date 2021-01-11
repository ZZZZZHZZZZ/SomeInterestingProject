# Initializer_list Problem
```C++
initializer_list<int> createInitList() {
	auto x = { 1,2,3 };
	return x;
}
int main() {
	initializer_list<int> item = createInitList();
	for (auto iter = item.begin(); iter != item.end(); ++iter) {
		cout << *iter << " ";
	}
}
//function out: -858993460 -858993460 -858993460
```
原因为initializer_list返回的只是两个指针，并且其内容是构建在栈中而不是堆中

导致返回后指针指向的位置对象已被delete,从而无法获取内容

// 题目：切片 append 与共享底层数组。练习约定：a 的初始 len=cap=3。
// b=a[:2]：append 4 覆盖 a[2]；再 append 5 超过 cap，b 使用新数组；改 b[0] 不再影响 a。
// 若初始 cap>3，后续是否共享会不同；不依赖扩容后容量的具体数值。
// append 摊还 O(1)，扩容 O(n)。运行：go run go_slice_append.go
package main

import "fmt"

func main() {
	a := []int{1, 2, 3}
	b := a[:2]
	fmt.Println("initial a:", a, "len/cap:", len(a), cap(a))
	b = append(b, 4)
	fmt.Println("append 4, a:", a, "b:", b)
	b = append(b, 5)
	fmt.Println("append 5, a:", a, "b:", b)
	b[0] = 10
	fmt.Println("b[0]=10, a:", a, "b:", b)
}

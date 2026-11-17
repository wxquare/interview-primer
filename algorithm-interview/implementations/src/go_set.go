// 题目：Go 实现 Set。练习约定：泛型键必须 comparable；不保证遍历顺序，不提供并发安全。
// map[T]struct{} 只表达键是否存在，struct{} 没有业务载荷；不承诺具体运行时桶布局。
// 增删查平均 O(1)，空间 O(n)。运行：go run go_set.go（Go 1.18+）。
package main

import "fmt"

type Set[T comparable] struct{ values map[T]struct{} }

func (s *Set[T]) Add(value T) {
	if s.values == nil {
		s.values = make(map[T]struct{})
	}
	s.values[value] = struct{}{}
}
func (s *Set[T]) Contains(value T) bool { _, ok := s.values[value]; return ok }
func (s *Set[T]) Remove(value T)        { delete(s.values, value) }
func (s *Set[T]) Len() int              { return len(s.values) }
func (s *Set[T]) Values() []T {
	values := make([]T, 0, len(s.values))
	for value := range s.values {
		values = append(values, value)
	}
	return values
}
func main() {
	var set Set[string]
	set.Add("Go")
	set.Add("Go")
	set.Add("C++")
	fmt.Println(set.Len(), set.Contains("Go"))
	set.Remove("Go")
	fmt.Println(set.Contains("Go"))
}

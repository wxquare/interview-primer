// 题目：3 个 goroutine 顺序打印 123，循环 100 次；令牌按 1→2→3 传递。
// 最后一个 3 不再传令牌；不靠 sleep 排序。时间 O(轮数)，空间 O(1)。
// 运行：go run concurrency_print_123.go
package main

import (
	"fmt"
	"sync"
)

func main() {
	const rounds = 100
	turns := [3]chan struct{}{make(chan struct{}, 1), make(chan struct{}, 1), make(chan struct{}, 1)}
	var wg sync.WaitGroup
	for id := 0; id < 3; id++ {
		wg.Add(1)
		go func(id int) {
			defer wg.Done()
			for round := 0; round < rounds; round++ {
				<-turns[id]
				fmt.Print(id + 1)
				if id != 2 || round != rounds-1 {
					turns[(id+1)%3] <- struct{}{}
				}
			}
		}(id)
	}
	turns[0] <- struct{}{}
	wg.Wait()
	fmt.Println()
}

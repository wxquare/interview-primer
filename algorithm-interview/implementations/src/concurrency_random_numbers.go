// 题目：两个 goroutine，一个生产、一个消费并打印，最终输出 5 个随机数。
// 只有生产者关闭 channel；main 等待两个 worker，避免提前退出；不要求密码学随机。
// 时间 O(n)，无缓冲 channel 额外空间 O(1)。运行：go run concurrency_random_numbers.go
package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

func main() {
	numbers := make(chan int)
	var wg sync.WaitGroup
	wg.Add(2)
	go func() {
		defer wg.Done()
		defer close(numbers)
		random := rand.New(rand.NewSource(time.Now().UnixNano()))
		for i := 0; i < 5; i++ {
			numbers <- random.Intn(100)
		}
	}()
	go func() {
		defer wg.Done()
		for number := range numbers {
			fmt.Println(number)
		}
	}()
	wg.Wait()
}

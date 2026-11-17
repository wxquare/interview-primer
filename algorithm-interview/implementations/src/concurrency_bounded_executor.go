// 题目：最多 10 个 goroutine 执行任务，每完成一个立即补下一个。
// 固定 10 个 worker 领取任务，不分批等待；任务需自行终止且不 panic，不提供超时/取消。
// 练习约定：限制任务 worker 数，不包含调用方 main。调度 O(n)，结果空间 O(n)。
// 运行：go run concurrency_bounded_executor.go
package main

import (
	"fmt"
	"sync"
)

func Execute(tasks []func() error) []error {
	results := make([]error, len(tasks))
	jobs := make(chan int)
	workers := len(tasks)
	if workers > 10 {
		workers = 10
	}
	var wg sync.WaitGroup
	for i := 0; i < workers; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for index := range jobs {
				if tasks[index] == nil {
					results[index] = fmt.Errorf("nil task")
					continue
				}
				results[index] = tasks[index]()
			}
		}()
	}
	for index := range tasks {
		jobs <- index
	}
	close(jobs)
	wg.Wait()
	return results
}
func main() {
	tasks := make([]func() error, 25)
	for i := range tasks {
		id := i
		tasks[i] = func() error { fmt.Println("task", id); return nil }
	}
	fmt.Println("results:", len(Execute(tasks)))
}

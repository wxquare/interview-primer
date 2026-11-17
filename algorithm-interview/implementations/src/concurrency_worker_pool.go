// 题目：协程池实现思路的可运行练习。固定 worker、有界任务队列、取消和收尾。
// 练习约定：批量任务接口；任务必须响应 ctx；退出前等待已开始任务，panic 转成任务错误。
// 未调度任务返回 ErrNotStarted。时间取决于任务耗时，调度 O(n)，空间 O(n+队列+workers)。
// 运行：go run concurrency_worker_pool.go
package main

import (
	"context"
	"errors"
	"fmt"
	"sync"
)

type Job func(context.Context) error

var ErrNotStarted = errors.New("task was not started")

func callJob(ctx context.Context, job Job) (err error) {
	defer func() {
		if value := recover(); value != nil {
			err = fmt.Errorf("task panic: %v", value)
		}
	}()
	if job == nil {
		return errors.New("nil task")
	}
	return job(ctx)
}
func RunPool(ctx context.Context, workers, queueCapacity int, tasks []Job) ([]error, error) {
	if workers <= 0 || queueCapacity < 0 {
		return nil, errors.New("invalid pool parameters")
	}
	results := make([]error, len(tasks))
	for i := range results {
		results[i] = ErrNotStarted
	}
	jobs := make(chan int, queueCapacity)
	var wg sync.WaitGroup
	for worker := 0; worker < workers; worker++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for index := range jobs {
				if err := ctx.Err(); err != nil {
					results[index] = err
					continue
				}
				results[index] = callJob(ctx, tasks[index])
			}
		}()
	}
dispatch:
	for index := range tasks {
		select {
		case <-ctx.Done():
			break dispatch
		case jobs <- index:
		}
	}
	close(jobs) // 调度方是唯一发送方和关闭方。
	wg.Wait()   // 不持锁执行任务；各 worker 只写自己的任务结果槽位。
	return results, ctx.Err()
}
func main() {
	tasks := []Job{
		func(ctx context.Context) error { fmt.Println("job A"); return nil },
		func(ctx context.Context) error { fmt.Println("job B"); return nil },
	}
	results, err := RunPool(context.Background(), 2, 2, tasks)
	fmt.Println(results, err)
}

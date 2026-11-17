// 题目：Go 1000 并发与 1 秒超时。练习约定：1000 是任务并发上限，1 秒是整个批次期限。
// 如需“每任务 1 秒”，应在 worker 内分别创建 timeout，语义不同。
// 超时立即返回已收到结果的数量，未完成任务通过 ctx 合作退出；Go 无法强杀不响应取消的 goroutine。
// 调度 O(n)，空间 O(n+min(n,1000))。运行：go run concurrency_batch_timeout.go
package main

import (
	"context"
	"fmt"
	"sync"
	"time"
)

type Task func(context.Context) error

func RunBatch(parent context.Context, tasks []Task) (int, []error, error) {
	ctx, cancel := context.WithTimeout(parent, time.Second)
	defer cancel()
	workers := len(tasks)
	if workers > 1000 {
		workers = 1000
	}
	jobs := make(chan Task)
	results := make(chan error, len(tasks))
	var wg sync.WaitGroup
	for i := 0; i < workers; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for task := range jobs {
				if ctx.Err() != nil {
					return
				}
				if task == nil {
					results <- fmt.Errorf("nil task")
					continue
				}
				results <- task(ctx)
			}
		}()
	}
	go func() {
		defer close(jobs)
		for _, task := range tasks {
			select {
			case <-ctx.Done():
				return
			case jobs <- task:
			}
		}
	}()
	go func() { wg.Wait(); close(results) }()
	completed := 0
	failures := make([]error, 0)
	for {
		select {
		case <-ctx.Done():
			return completed, failures, ctx.Err()
		case err, ok := <-results:
			if !ok {
				return completed, failures, ctx.Err()
			}
			completed++
			if err != nil {
				failures = append(failures, err)
			}
		}
	}
}
func main() {
	tasks := make([]Task, 1200)
	for i := range tasks {
		tasks[i] = func(ctx context.Context) error {
			timer := time.NewTimer(10 * time.Millisecond)
			defer timer.Stop()
			select {
			case <-ctx.Done():
				return ctx.Err()
			case <-timer.C:
				return nil
			}
		}
	}
	done, failures, err := RunBatch(context.Background(), tasks)
	fmt.Println("completed:", done, "failures:", len(failures), "batch error:", err)
}

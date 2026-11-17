// 题目：并发查询可用支付方式。练习约定：调用方提供整体 deadline；远程 check 必须响应 ctx。
// 余额/红包等保持各自远程调用，不假设能合并资源；去重后有界并发，超时返回已收到的可用结果。
// 单项失败保存在 errors；按输入顺序返回，错误/超时不等同于不可用。
// 调度 O(n)，空间 O(n+parallel)。运行：go run concurrency_payment_queries.go
package main

import (
	"context"
	"errors"
	"fmt"
	"sync"
	"time"
)

type EnabledFunc func(context.Context, string) (bool, error)
type paymentResult struct {
	kind    string
	enabled bool
	err     error
}
type PaymentSummary struct {
	Available []string
	Errors    map[string]error
	Completed int
	Err       error
}

func AvailablePayments(ctx context.Context, kinds []string, parallel int, check EnabledFunc) PaymentSummary {
	summary := PaymentSummary{Errors: make(map[string]error)}
	if parallel <= 0 || check == nil {
		summary.Err = errors.New("invalid query parameters")
		return summary
	}
	unique := make([]string, 0, len(kinds))
	seen := make(map[string]bool)
	for _, kind := range kinds {
		if !seen[kind] {
			seen[kind] = true
			unique = append(unique, kind)
		}
	}
	child, cancel := context.WithCancel(ctx)
	defer cancel()
	jobs := make(chan string)
	results := make(chan paymentResult, len(unique)) // 调用方超时退出后，worker 发送不会阻塞。
	var wg sync.WaitGroup
	if parallel > len(unique) {
		parallel = len(unique)
	}
	for i := 0; i < parallel; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for kind := range jobs {
				if child.Err() != nil {
					return
				}
				enabled, err := check(child, kind)
				results <- paymentResult{kind, enabled, err}
			}
		}()
	}
	go func() {
		defer close(jobs)
		for _, kind := range unique {
			select {
			case <-child.Done():
				return
			case jobs <- kind:
			}
		}
	}()
	go func() { wg.Wait(); close(results) }()
	available := make(map[string]bool)
	record := func(result paymentResult) {
		summary.Completed++
		if result.err != nil {
			summary.Errors[result.kind] = result.err
		} else if result.enabled {
			available[result.kind] = true
		}
	}
collect:
	for {
		select {
		case result, ok := <-results:
			if !ok {
				break collect
			}
			record(result)
		case <-child.Done():
			summary.Err = child.Err()
			// 回收已经到达缓冲区的结果；不继续等待远程服务。
			for {
				select {
				case result, ok := <-results:
					if !ok {
						break collect
					}
					record(result)
				default:
					break collect
				}
			}
		}
	}
	for _, kind := range unique {
		if available[kind] {
			summary.Available = append(summary.Available, kind)
		}
	}
	return summary
}
func main() {
	ctx, cancel := context.WithTimeout(context.Background(), 200*time.Millisecond)
	defer cancel()
	check := func(ctx context.Context, kind string) (bool, error) {
		timer := time.NewTimer(20 * time.Millisecond)
		defer timer.Stop()
		select {
		case <-ctx.Done():
			return false, ctx.Err()
		case <-timer.C:
			return kind != "voucher", nil
		}
	}
	fmt.Printf("%+v\n", AvailablePayments(ctx, []string{"balance", "red_packet", "coupon", "voucher"}, 4, check))
}

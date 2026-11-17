// 题目：手写多生产者、多消费者消息队列。练习约定：进程内、有界 int 环形队列。
// mutex + 两个 Cond 分别等待非空/非满；Close 后拒绝 Put，Get 排空后返回 false。
// 全部生产者退出后由协调方 Close；Close 可重复，唤醒阻塞的生产者/消费者。
// Put/Get O(1)（不含等待），空间 O(capacity)；不提供持久化或处理失败后的重投递。
// 运行：go run concurrency_mpmc_queue.go
package main

import (
	"fmt"
	"sync"
)

type BoundedQueue struct {
	mutex    sync.Mutex
	notEmpty *sync.Cond
	notFull  *sync.Cond
	buffer   []int
	head     int
	size     int
	closed   bool
}

func NewBoundedQueue(capacity int) *BoundedQueue {
	if capacity <= 0 {
		panic("capacity must be positive")
	}
	queue := &BoundedQueue{buffer: make([]int, capacity)}
	queue.notEmpty = sync.NewCond(&queue.mutex)
	queue.notFull = sync.NewCond(&queue.mutex)
	return queue
}

// 队列包含 mutex/Cond，初始化后只能通过指针使用，不可复制。
func (q *BoundedQueue) Put(value int) bool {
	q.mutex.Lock()
	defer q.mutex.Unlock()
	for q.size == len(q.buffer) && !q.closed {
		q.notFull.Wait()
	}
	if q.closed {
		return false
	}
	tail := (q.head + q.size) % len(q.buffer)
	q.buffer[tail] = value
	q.size++
	q.notEmpty.Signal()
	return true
}

func (q *BoundedQueue) Get() (int, bool) {
	q.mutex.Lock()
	defer q.mutex.Unlock()
	for q.size == 0 && !q.closed {
		q.notEmpty.Wait()
	}
	if q.size == 0 {
		return 0, false
	}
	value := q.buffer[q.head]
	q.head = (q.head + 1) % len(q.buffer)
	q.size--
	q.notFull.Signal()
	return value, true
}

func (q *BoundedQueue) Close() {
	q.mutex.Lock()
	defer q.mutex.Unlock()
	q.closed = true
	q.notEmpty.Broadcast()
	q.notFull.Broadcast()
}

func main() {
	const producers, consumers, perProducer = 3, 2, 5
	queue := NewBoundedQueue(4)
	totals := make([]int, consumers)
	var producersDone, consumersDone sync.WaitGroup
	for c := 0; c < consumers; c++ {
		consumersDone.Add(1)
		go func(index int) {
			defer consumersDone.Done()
			for {
				value, ok := queue.Get()
				if !ok {
					return
				}
				totals[index] += value
			}
		}(c)
	}
	for p := 0; p < producers; p++ {
		producersDone.Add(1)
		go func(id int) {
			defer producersDone.Done()
			for i := 0; i < perProducer; i++ {
				if !queue.Put(id*perProducer + i) {
					return
				}
			}
		}(p)
	}
	producersDone.Wait()
	queue.Close()
	consumersDone.Wait()
	total := 0
	for _, subtotal := range totals {
		total += subtotal
	}
	fmt.Println("sum:", total)
}

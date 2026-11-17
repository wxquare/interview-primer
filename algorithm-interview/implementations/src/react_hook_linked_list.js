/* 题目：用链表体现 React Hook 的调用顺序约束。
 * 练习模型：useState 节点 + 更新队列 + 工作链表提交，非 React 完整实现；手动 render。
 * 每次 render 按上次顺序消费节点，多/少 Hook 都报错；相同数量下交换语义也可能错绑状态。
 * 更新失败不提交状态/消费队列；不支持 render 内 setState，也不实现调度和并发渲染。
 * 一次渲染 O(Hook 数+更新数)，额外空间 O(Hook 数)。运行：node react_hook_linked_list.js
 */
function createRenderer(component) {
  let head = null, mounted = false, rendering = false;
  let cursor = null, draftHead = null, draftTail = null, consumed = [];

  function useState(initial) {
    if (!rendering) throw new Error('Hook must be called during render');
    let state, queue;
    if (!mounted) {
      state = typeof initial === 'function' ? initial() : initial;
      queue = { pending: [], setState: null };
      queue.setState = update => {
        if (rendering) throw new Error('setState during render is outside this exercise');
        queue.pending.push(update);
      };
    } else {
      if (cursor === null) throw new Error('Rendered more Hooks than previous render');
      state = cursor.memoizedState;
      queue = cursor.queue;
      cursor = cursor.next;
      for (const update of queue.pending) state = typeof update === 'function' ? update(state) : update;
      consumed.push([queue, queue.pending.length]);
    }
    const node = { memoizedState: state, queue, next: null };
    if (draftTail) draftTail.next = node; else draftHead = node;
    draftTail = node;
    return [state, queue.setState];
  }

  function render(props) {
    if (rendering) throw new Error('Recursive render is unsupported');
    rendering = true; cursor = head; draftHead = null; draftTail = null; consumed = [];
    try {
      const output = component(useState, props);
      if (mounted && cursor !== null) throw new Error('Rendered fewer Hooks than previous render');
      head = draftHead; mounted = true;
      for (const [queue, count] of consumed) queue.pending.splice(0, count);
      return output;
    } finally {
      rendering = false; cursor = null; draftHead = null; draftTail = null; consumed = [];
    }
  }
  return { render };
}

if (require.main === module) {
  const renderer = createRenderer((useState, props) => {
    const [count, setCount] = useState(0);
    if (props.extra) useState('extra');
    return { count, setCount };
  });
  const first = renderer.render({ extra: true });
  first.setCount(count => count + 1);
  console.log(renderer.render({ extra: true }).count);
  try { renderer.render({ extra: false }); } catch (error) { console.log(error.message); }
}
module.exports = { createRenderer };

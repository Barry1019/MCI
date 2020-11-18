public class Main{
	public static void main(String[] args) {
		// MyStack s1 = new MyStack();
		// s1.push(new Node("s"));
		// s1.push(new Node("m"));
		// System.out.println(s1.getSize());
		// s1.pop().printNode();
		// s1.pop().printNode();
		// System.out.println(s1.pop());
		System.out.println(bracketsMatching("({})"));
		System.out.println(bracketsMatching("({)"));
		System.out.println(bracketsMatching("(1+2) * {(2+3)*(3+4)}"));
		System.out.println(bracketsMatching("(1+2) * {{2+3)*(3+4}}"));

		// MyQueue q1 = new MyQueue();
		// q1.enqueue(new Node("s"));
		// q1.enqueue(new Node("m"));
		// System.out.println(q1.getSize());
		// System.out.println(q1.dequeue());
		// System.out.println(q1.dequeue());
		// System.out.println(q1.dequeue());

		//  test 1
		MyQueue test1 = new MyQueue(new Node("1"), new Node("4"));
		test1.enqueue(new Node("7"));
		test1.enqueue(new Node("9"));
		test1.enqueue(new Node("2"));
		test1.displayQueue();
		reverseQueue(test1).displayQueue();

		//  test 2
		MyQueue test2 = new MyQueue(new Node("3"), new Node("3"));
		test2.enqueue(new Node("4"));
		test2.enqueue(new Node("1"));
		test2.enqueue(new Node("2"));
		test2.displayQueue();
		reverseQueue(test2).displayQueue();
	}

	public static boolean bracketsMatching(String input){
		MyStack brackets = new MyStack();

		for (int i = 0; i < input.length(); i++) {
			String current = input.substring(i, i+1);

			if (current.equals("{") || current.equals("[") || current.equals("(")) {
				brackets.push(new Node(current));  //  add the left part to the stack
			}else if (current.equals("}") || current.equals("]") || current.equals(")")) {
				if (brackets.isEmpty()) {
					return false;  //  no left only right, cannot match
				}else{
					String last = brackets.peek().getData();
					if (current.equals("}") && last.equals("{") 
						|| current.equals("]") && last.equals("[")
						|| current.equals(")") && last.equals("(")) {
						brackets.pop();  //  the inner two brackets match, pop the left part
					}else{
						return false;  //  the inner two brackets cannot match
					}
				}
			}
		}
		return brackets.isEmpty();  //  all the left parts poped due to the right, then it matches
	}

	//  takes a Myqueue object as input, and return a Myqueue object which saves a reversed queue
	public static MyQueue reverseQueue(MyQueue queue){
		MyStack tmp = new MyStack();
		while(!queue.isEmpty()){
			tmp.push(new Node(queue.dequeue()));
		}  //  store the queue reversely in a stack
		while(!tmp.isEmpty()){
			queue.enqueue(tmp.pop());
		}  //  put the Nodes back to the queue
		return queue;
	}
}
public class MyStack{
	private Node top; // The reference to the top node in this stack

	// initialise an empty stack in the constructor
	public MyStack(){

	}

	//  a method to get the size of Mystack
	public int getSize(){
		Node currentNode = this.top;
		int count = 0;

		if (currentNode == null) {
			return 0;
		}else{
			while(currentNode != null){
				count++;
				currentNode = currentNode.getNext();
			}
			return count;
		}
	}

	// Push a node into stack
	public void push(Node node){
		if (isEmpty()) {
			this.top = node;
		}else{
			//  add new elements on the top
			Node tmp = this.top;
			this.top = node;
			this.top.setNext(tmp);
		}
	}

	// Get and remove the top node from this stack.
	// Return Null and print "Stack is empty" when the stack is empty.
	public Node pop(){
		if (isEmpty()) {
			System.out.println("Stack is empty");
			return null;
		}else {
			Node tmp = this.top;
			this.top = this.top.getNext();
			return tmp;
		}
	}

	// Get the top node in this stack. 
	// Return Null and print "Stack is empty" when the stack is empty.
	public Node peek(){
		if (isEmpty()) {
			System.out.println("Stack is empty");
			return null;
		}else{
			return this.top;
		}
	}

	// Return TRUE when the stack is empty, otherwise, return FALSE.
	public boolean isEmpty(){
		if (this.top == null) {
			return true;
		}else{
			return false;
		}
	}
}
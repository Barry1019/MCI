public class MyQueue {
	private Node front, rear; // The reference to the first and last node in this queue

	//  initialise the an empty queue in the constructor
	public MyQueue(){
		
	}
	//  Parametrized constructor
	public MyQueue(Node front, Node rear){
		this.front = front;
		this.rear = rear;
		this.front.setNext(this.rear);
	}

	//  a method to get the size of MyQueue
	public int getSize(){
		Node currentNode = this.front;
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

	// Insert one node at the end of the queue.
	public void enqueue(Node node){
		if (isEmpty()) {
			this.front = node;
			this.rear = node;
		}else{
			this.rear.setNext(node);
			this.rear = node;
		}
	}

	// Get and remove the front node from the queue. 
	// Return Null and print "Queue is empty" when the queue is empty
	public String dequeue(){
		if (isEmpty()) {
			System.out.println("Queue is empty");
			return null;
		}else{
			Node tmp = this.front;
			this.front = this.front.getNext();
			return tmp.getData();
		}
	}

	// Return TRUE when the stack is empty, otherwise, return FALSE.
	public boolean isEmpty(){
		if (this.front == null) {
			return true;
		}else{
			return false;
		}
	}

	// Print out the data saved in nodes from the first to the last. 
	// Return and print "Queue is empty" when the queue is empty
	public void displayQueue(){
		if (isEmpty()) {
			System.out.println("Queue is empty");
		}else{
			Node tmp = this.front;
			while(tmp != null){
				tmp.printNode();
				tmp = tmp.getNext();
			}
		}
	}
}
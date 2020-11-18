public class Node{
	private String data; // the data saved in this node
	private Node next; // the reference to another node

	//  Basic constructor
	Node(){
		this.setData(null);
		this.setNext(null);
	}

	//  Parametrized constructor
	Node(String data){
		this.setData(data);
		this.setNext(null);
	}

	//  Mutators / setters
	public void setData(String data){
		this.data = data;
	}

	public void setNext(Node next){
		this.next = next;
	}

	//  Accessors / getters
	public String getData(){
		return this.data;
	}

	public Node getNext(){
		return this.next;
	}

	// Print the data saved in the node. 
	public void printNode(){
		System.out.print("Node data: \"");
		System.out.print(this.getData());
		System.out.println("\"");
	}
}
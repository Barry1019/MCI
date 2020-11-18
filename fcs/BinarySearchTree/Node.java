//student ID: a1790846
//student name: Shaokang Ma

public class Node {
	//  attributes
	private int data; // the data saved in this node
	private Node left; // the reference to its left child
	private Node right; // the reference to its right child

	//  Basic constructor
	public Node(){
		this.setData(0);
		this.left = null;
		this.right = null;
	}

	//  Parametrized constructor
	public Node(int data){
		this();
		this.setData(data);
	}

	//  Mutators / setters
	public void setData(int data){
		this.data = data;
	}

	public void setLeft(int left){
		this.left = new Node(left);
	}

	public void setRight(int right){
		this.right = new Node(right);
	}

	public void setLeft(Node left){
		this.left = left;
	}

	public void setRight(Node right){
		this.right = right;
	}

	//  Accessors / getters
	public int getData(){
		return this.data;
	}

	public Node getLeft(){
		return this.left;
	}

	public Node getRight(){
		return this.right;
	}

	// Print the data saved in the node. 
	public void printNode(){
		System.out.print("Node data: \"");
		System.out.print(this.getData());
		System.out.println("\"");
	}
}
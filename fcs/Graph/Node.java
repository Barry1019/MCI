//student ID: a1790846
//student name: Shaokang Ma

public class Node {
	//  attribute
	private int index; // the index of this node

	//  Basic constructor
	public Node(){
		this.setIndex(0);
	}

	//  Parametrized constructor
	public Node(int index){
		this.setIndex(index);
	}

	//  Mutators / setters
	public void setIndex(int index){
		this.index = index;
	}

	//  Accessors / getters
	public int getIndex(){
		return this.index;
	}

	// Print the data (the index) saved in the node. 
	public void printNode() {
		System.out.print("Node ");
		System.out.print(this.getIndex());
	}
}
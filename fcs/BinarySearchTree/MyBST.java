//student ID: a1790846
//student name: Shaokang Ma

public class MyBST {
	//  attribute
	private Node root; // The reference to the root node in this tree

	//  Basic constructor
	public MyBST(){
		this.root = null;
	}

	//  Parametrized constructor
	public MyBST(int root){
		this.root = new Node(root);
	}

	// Insert a new value into the tree. This method calls insertRec()
	public void insert(int value) {
		if (this.root == null) {
			this.root = new Node(value);
		}else{
			this.insertRec(this.root, value);
		}
	}

	// This is a recursive function to insert a new value in the tree.
	//  Assume the root is not null
	//  Always try to insert to the next level, so the reference can be kept
	private void insertRec(Node current, int value) {
		if (value < current.getData()) {
			if (current.getLeft() == null) {
				current.setLeft(value);
			}else{
				insertRec(current.getLeft(), value);
			}
		}  //  if value is smaller, go to the left
		else if (value > current.getData()) {
			if (current.getRight() == null) {
				current.setRight(value);
			}else{
				insertRec(current.getRight(), value);
			}
		}  //  if value is bigger, go to the right
		else{
			System.out.println("Node is in the tree");
			return;
		}  //  if a node is already in this tree
	}

	// Search a node in the tree. This method calls searchRec()
	public boolean search(int value) {
		return this.searchRec(this.root, value);
	}

	// This is a recursive function to search a node in the tree.
	private boolean searchRec(Node current, int value) {
		if (current == null) {
			return false;
		}else if (current.getData() == value) {
			return true;
		}  //  two basic cases
		else if (value < current.getData()) {
			return searchRec(current.getLeft(), value);
		}else{
			return searchRec(current.getRight(), value);
		}
	}

	//  Print out all elements in the tree
	public void printBST(Node root){
		if (root != null) {
			printBST(root.getLeft());
			System.out.print(root.getData() + " ");
			printBST(root.getRight());
		}
	}

	public void print(){
		this.printBST(this.root);
		System.out.println();
	}
}
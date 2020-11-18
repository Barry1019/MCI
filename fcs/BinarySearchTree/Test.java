//student ID: a1790846
//student name: Shaokang Ma

public class Test{
	public static void main(String[] args) {
		MyBST b1 = new MyBST(3);
		b1.print();
		b1.insert(1);
		b1.print();
		b1.insert(4);
		b1.print();
		b1.insert(2);
		b1.print();
		b1.insert(1);

		System.out.println(b1.search(2));
	}
}
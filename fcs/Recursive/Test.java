public class Test{
	public static void main(String[] args) {
		Main m = new Main();
		System.out.println(m.nextHappyNum(8));
		System.out.println(m.nextHappyNum(3));
		System.out.println(m.nextHappyNum(0));
		System.out.println(m.nextHappyNum(10));

		System.out.println(m.decodeString("3[b2[ca]1[d]]"));
	}
}
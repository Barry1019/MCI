public class Main{
	public static void main(String[] args) {
		int[] array = {1,5,8,12,9,7,-1};
		int ans1 = 5;  //  the index of element "7"
		int ans2 = -1;  //  the index when the element does not exist
		MySearchAlg l1 = new LinearSearch();
		MySearchAlg b1 = new BinarySearch();
		int result1 = l1.search(array, 7);
		int result2 = b1.search(array, 7);
		int result3 = b1.search(array, 2);  //  2 does not exist in the array

		boolean test1 = test(result1, ans1);
		boolean test2 = test(result2, ans2);
		boolean test3 = test(result3, ans2);

		//  3 tests
		System.out.println(test1);
		System.out.println(test2);
		System.out.println(test3);

		//  test the findMaxVal() method
		int[] array1 = {1,5,8,12,9,7,-1};
		int[] array2 = {1,15,0};

		System.out.println(findMaxVal(array1));
		System.out.println(findMaxVal(array2));
	}

	//  a method to test whether the search algorithm works
	private static boolean test(int result, int ans){
		if (result == ans) {
			return true;
		}else{
			return false;
		}
	}

	private static int findMaxVal(int array[]){
		MySearchAlg l1 = new LinearSearch();
		int[] sortedArray = l1.sort(array);  //  first sort the inpuy in ascending order without changing it
		int n = array.length;
		int max = sortedArray[n-1];  //  the last element must be the max

		return l1.search(array, max);
	}
}
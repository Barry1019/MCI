//student ID: a1790846
//student name: Shaokang Ma

public class Main{
	public static void main(String[] args) {
		//  test the test() method
		int[] testArray = {4,2,3,1};
		int[] ansArray = {4,3,2,1};

		InsertionSort sorted1 = new InsertionSort();
		MergeSort sorted2 = new MergeSort();
		QuickSort sorted3 = new QuickSort();

		//  test 3 times
		System.out.println(test(sorted1.sort(testArray), ansArray));
		System.out.println(test(sorted2.sort(testArray), ansArray));
		System.out.println(test(sorted3.sort(testArray), ansArray));

		// test the compare() method
		int[] compare1 = {1,3,5,2,4};
		int[] compare2 = {2,4,1,3,5};
		System.out.println(compare(compare1, compare2));

		//  test the findSmallestSum() method
		int[] smallerSum = {4,7,0,0,2,9,10};
		System.out.println(findSmallestSum(smallerSum));
	}

	//  a method to test my sorting algs
	public static boolean test(int[] result, int[] ans){
			int n1 = result.length;
			int n2= ans.length;

			//  Compare the length
			if (n1 != n2) {
				return false;
			}else{
				int i = 0;
				while (i < n1) {
					if (result[i] != ans[i]) {
						break;
					}else{
						i++;
					}
				}

				if (i == n1) {
					return true;
				}else{
					return false;
				}
			}
	}

	//  a method to compare 2 arrays (using test() method)
	public static boolean compare(int[] arr1, int[] arr2){
		InsertionSort insertionSort = new InsertionSort();
		int[] sortedArr1 = insertionSort.sort(arr1);
		int[] sortedArr2 = insertionSort.sort(arr2);

		return test(sortedArr1, sortedArr2);
	}

	//  Get the smallest sum of two elements in an array (>= 2elements)
	public static int findSmallestSum(int[] array){
		InsertionSort insertionSort = new InsertionSort();
		int[] result = insertionSort.sort(array);
		int n = result.length;
		int sum = 0;

		for (int i = n - 2; i < n; i++) {
			sum = sum + result[i];
		}

		return sum;
	}
}
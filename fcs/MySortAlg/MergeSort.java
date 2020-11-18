public class MergeSort extends MySortAlg{
	public int[] sort(int[] array){
		int n = array.length;
		if (n < 2) {  //  base case
			return array;
		}else{
			int middle = Math.round(n/2.0f);

			//  Sort left part recursively
			int[] left = this.getSubArr(array, 0, middle);
			left = this.sort(left);

			//  Sort right part recursively
			int[] right = this.getSubArr(array, middle, n);
			right = this.sort(right);

			int[] merged = this.merge(left,right);
			return merged;
		}
	}

	//  a method to make sub array from start to end(excluded)
	private int[] getSubArr(int[] array, int start, int end){
		int length = end - start;

		if (length < 1) {
			int[] subArray = {};
			return subArray;
		}else{
			int[] subArray = new int[length];
			int j = 0;

			for (int i = start; i < end; i++) {
				subArray[j] = array[i];
				j++;
			}
			return subArray;
		}
	}

	private int[] merge(int[] left, int[] right){
		int n1 = left.length;
		int n2 = right.length;
		int[] result = new int[n1 + n2];

		int i1 = 0, i2 = 0, j = 0;
		while (i1 < n1 && i2 < n2) {
			if (left[i1] > right[i2]) {
				result[j] = left[i1];
				i1++;
			}else{
				result[j] = right[i2];
				i2++;
			}
			j++;
		}

		//  Add all the rest elements to result
		while(i1 < n1){
			result[j] = left[i1];
			j++;
			i1++;
		}

		while(i2 < n2){
			result[j] = right[i2];
			j++;
			i2++;
		}		

		return result;
	}
}
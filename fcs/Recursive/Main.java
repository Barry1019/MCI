public class Main{
	//  A method to find the next smallest Happy Number
	int nextHappyNum(int num){
		if (num <= 1) {
			return 1;
		}else{
			num++;
			while(!isHappyNumber(num)){
				num++;
			}
			return num;
		}
	}

	//  The core is showen as below, name the method A() for convenience
	//  A(13[ab6[ab]7[ab]]) ->
	//  inner = decode(A)
	//  decodeString(inner)
	//  until there is no "["/"]" but only pure letters

	//  a method to decode one-level String
	private String decode(String s) {
		StringBuilder sb = new StringBuilder();
		int index = 0;
		String tmp = "[]";
		int n = s.length();

		//  decode the whole line until no"["/"]" on the same level
		while(index < n) {
			while(s.charAt(index) > tmp.charAt(1)) {
				sb.append(s.charAt(index));
				index++;
			}
			
			int nextL = s.indexOf("[", index);
			String timesString = s.substring(index,nextL);
			int times = Integer.parseInt(timesString);
			int nextR = getNextInner(s, index);
			index = nextL + 1;
			String repeat = s.substring(index, nextR);
			
			for(int i = 0; i < times; i++) {
				sb.append(repeat);
			}
			index = nextR + 1;
		}
		
		return sb.toString();
	}

	//  a method to decode the string repeatedly
	String decodeString(String s){
		//  base case
		if(isPure(s)) {
			return s;
		}else {
			String inner = decode(s);
			return decodeString(inner);
		}
	}

	//  A method to sum up all the squares of digits
	private int sqr(int num){
		if (num < 10) {
			return num * num;
		}else{
			int sum = 0;
			while(num >= 10){
				//  sum up the square of the last digit
				int remainder = num % 10;
				sum += remainder * remainder;
				num = num / 10;  //  dispose of the last digit
			}

			sum += num * num;
			return sum;
		}
	}

	//  A method to tell whether a number is a Happy Number
	private boolean isHappyNumber(int num){
		//  two base cases
		if (num <= 0) {
			return false;
		}else if (sqr(num) == 1) {
			return true;
		//  unhappy number will go into the loop: 4 → 16 → 37 → 58 → 89 → 145 → 42 → 20 → 4 
		}else if (sqr(num) == 4 || sqr(num) == 16 || sqr(num) == 37 
			|| sqr(num) == 58 || sqr(num) == 89 || sqr(num) == 145 
			|| sqr(num) == 42 || sqr(num) == 20 || sqr(num) == 4) {
			return false;
		}else{
			num = sqr(num);
			return isHappyNumber(num);
		}
	}

	//  several methods to tell the type of an element
	//  ***IMPORTANT: z>y>x> ... >a>]>[>9>8> ... >1***
	//  ***REMEMBER ALWAYS "", such as "9", when comparing by charAt()***
	//  a method to tell if the element is a number
	//  a method to tell if the String has no number
	private static boolean isPure(String s){
		String st = "]";
		int n = s.length();
		boolean isPure = true;

		for (int i = 0; i < n; i++) {
			if (s.charAt(i) <= st.charAt(0)) {
				isPure = false;
				break;
			}
		}
		return isPure;
	}

	//  A method to find out boundary of an inner String
	private int getNextInner(String st, int index){
		String s = "[]";
		int countL = 0;  //  represents the number of "["
		int countR = 0;  //  represents the number of "]"
		int n = st.length();

		if (isPure(st)) {
			return -1;
		}else{
			int i;
			for (i = index; i < n; i++) {
				if (st.charAt(i) == s.charAt(0)){
					countL++;
				}else if (st.charAt(i) == s.charAt(1)) {
					countR++;
				}
				if (countL >0 && countL == countR) {
					break;
				}
			}
			return i;
		}
	}
}
//==================================
// Foundations of Computer Science
// Student: Shaokang Ma
// id: a1790846
// Semester: 1
// Year: 2020
// Practical Number: 04 - Part II
//===================================

import java.util.*;

public class ShakingPartyConstrainted{
	//  attribute
	private int nCouples;

	//  Constructor
	ShakingPartyConstrainted(){
		Random ra = new Random();
		int random = ra.nextInt();
		this.setNCouples(random);
	}
	ShakingPartyConstrainted(int nTmp){
		this.setNCouples(nTmp);
	}

	//  Accessor / getter
	public int getNCouples(){
		return this.nCouples;
	}

	//  Mutator / setter
	public void setNCouples(int nCouples){
		this.nCouples = nCouples;
	}

	//  n couples should shake hands 3n-2 times more than n-1 couples
	public int countHandShakes(){
		if (this.getNCouples() < 1) {
			return 0;
		}else if (this.getNCouples() == 1) {
			return 1;
		}else{
			this.setNCouples(this.getNCouples() - 1);
			return (this.getNCouples() + 1) * 3 - 2 + countHandShakes();
		}
	}
}
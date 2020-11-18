//==================================
// Foundations of Computer Science
// Student: Shaokang Ma
// id: a1790846
// Semester: 1
// Year: 2020
// Practical Number: 04 - Part II
//===================================

import java.util.*;

public class ShakingParty{
	//  attribute
	private int nPeople;

	//  Constructor
	ShakingParty(){
		Random ra = new Random();
		int random = ra.nextInt();
		this.setNPeople(random);
	}
	ShakingParty(int nTmp){
		this.setNPeople(nTmp);
	}

	//  Accessor / getter
	public int getNPeople(){
		return this.nPeople;
	}

	//  Mutator / setter
	public void setNPeople(int nPeople){
		this.nPeople = nPeople;
	}

	public int countHandShakes(){
		if (this.getNPeople() < 2) {
			return 0;
		}else if (this.getNPeople() == 2) {
			return 1;
		}else{
			this.setNPeople(this.getNPeople()-1);
			return this.getNPeople() + countHandShakes();
		}
	}
}
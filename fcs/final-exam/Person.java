//==================================
// Foundations of Computer Science
// Student: Shaokang Ma
// id: a1790846
// Semester: 01
// Year: 2020
// Practical Exam Number: final
//===================================

public class Person {
	//  attributes representing a person's name and age
	private String name;
	private int age;
	
	//  Basic constructor
	public Person() {
		this.setName("anon");
		this.setAge(-1);
	}
	
	//  Parametrized constructor
	public Person(String name, int age) {
		this.setName(name);
		this.setAge(age);
	}
	
	//  Accessors / getters
	public String getName() {
		return name;
	}
	public int getAge() {
		return age;
	}
	
	//  Mutators / setters
	public void setName(String name) {
		this.name = name;
	}
	public void setAge(int age) {
		this.age = age;
	}
	
	//  return a string with the person's basic information
	public String toString() {
		String info = "Person: " + this.getName() + " is age: " + this.getAge();
		return info;
	}
}

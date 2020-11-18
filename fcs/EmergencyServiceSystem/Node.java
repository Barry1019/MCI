public class Node {
    protected Node next;
    protected Patient patient;

    public Node(Patient patient) {
        this.patient = patient;
        this.next = null;
    }

    public Node getNext() {
        return next;
    }

    public void setNext(Node next) {
        this.next = next;
    }

    public Patient getPatient() {
        return patient;
    }

    public void setPatient(Patient patient) {
        this.patient = patient;
    }

    public void printNode() {
        System.out.print("The patient's name is: ");
        System.out.println(this.patient.getName());
        System.out.print("The patient's phone number is: ");
        System.out.println(this.patient.getPhoneNumber());
        System.out.print("The patient's triage level is: ");
        System.out.println(this.patient.getTriageLevel());
        System.out.print("The patient's location is: ");
        System.out.println(this.patient.getLocation());
        System.out.print("The patient's ID is: ");
        System.out.println(this.patient.getId());
    }
}

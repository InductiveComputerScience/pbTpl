public class TestLauncher {
    public static void main(String [] args){
        int failures;
        failures = (int)tests.tests.test();
        System.out.println(failures);
    }
}

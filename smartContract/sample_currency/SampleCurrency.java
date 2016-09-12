
import java.util.Map;

public class SampleCurrency {

    public static void remit(Map<String,String> params){
        System.out.println("Hello in JAVA!");
        Repository.save("Mizuki", params.get("Mizuki"));
    }
}




// no package declaration
public class Repository {
    public static native void save(String key,String value);
    public static native void find(String key);
    public static native void update(String key,String value);
    public static native void remove(String key);
}


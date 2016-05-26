/*
 * Basic IO and simple int arithmetic
 */
public class Sample1{
    public static void main( String[] args ){
        int i = 4; //;index:1
        int j = 3; //;index:2
        i = SimpleIO.readInt();
        j = 9 + i * 8;
        SimpleIO.printString( "Result is " );
        SimpleIO.printInt( j );
        SimpleIO.println();
        return;
    }
}
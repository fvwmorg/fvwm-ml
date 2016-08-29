import java.awt.Frame;
import java.awt.event.KeyEvent;


public class KeyTest implements java.awt.event.KeyListener {

    public KeyTest() {
      Frame f = new Frame("KeyTest");
      f.setBounds(300,300, 300, 300);
      f.addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosing(java.awt.event.WindowEvent e) {System.exit(0);}
        });
      f.addKeyListener(this);
      f.show();

    }

    // implementation of java.awt.event.KeyListener interface

    /**
     *
     * @param param1 <description>
     */
    public void keyPressed(KeyEvent param1) {
      System.err.println("Pressed: " + param1);
    }

    /**
     *
     * @param param1 <description>
     */
    public void keyReleased(KeyEvent param1) {
      System.err.println("Released: " + param1);
    }

    /**
     *
     * @param param1 <description>
     */
    public void keyTyped(KeyEvent param1) {
      System.err.println("Typed: " + param1);
    }

    public static void main (String[] args) {
      new KeyTest();
    } // end of main ()

} // KeyTest


package draw;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.awt.Component;
import java.awt.Graphics;
import javax.swing.JFrame;
import javax.swing.JPanel;

/**
 *
 * @author strotz
 */
public class DrawPanel extends JPanel {
    public DrawPanel() {
        setLayout(null);
        setUp();
    } 
    
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        FrameMain.boards.board.draw(g, 700, 50, false);
        FrameMain.boards.completed.draw(g, 50, 50, true);
    }
    public void setUp() {
        for (Component c: getComponents())
            c.setSize(c.getPreferredSize());
        JFrame f = new JFrame(getName());
        f.setContentPane(this);
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        f.pack();
        f.setVisible(false);    
    }
    
}

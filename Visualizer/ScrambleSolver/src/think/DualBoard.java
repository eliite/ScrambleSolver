package think;


import draw.DrawPanel;
import draw.FrameMain;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import javax.swing.Timer;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author strotz
 */
public class DualBoard {
    public Board board, completed;
    public int matchCounter;
    private int[] counter;
    
    static private double x1 = 0, y1 = 0, x1g = 0, y1g = 0,
            x1o = 0, y1o = 0, a0 = 0;
    
    static public ArrayList<Integer> used;
    
    public int step;
    
    private int last_used;
    
    private Timer timer;
    
    public DualBoard() {
        board = new Board(9);
        board.init();
        
        completed = new Board(board.getSize());
        
        matchCounter = 0;
        step = 0;
        last_used = -1;
        
        ActionListener taskPerformer = new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                if (x1!=x1g)
                    x1 -= ((x1o-x1g)/10.0);
                if (y1!=y1g)
                    y1 -= ((y1o-y1g)/10.0);
                FrameMain.PanelDraw.repaint();
            }
        };
        
        timer = new Timer(100, taskPerformer);
        timer.setRepeats(true);
        timer.start();
        
        used = new ArrayList<Integer>();
   
        counter = new int[board.getSize()];
        for (int i = 0; i < board.getSize(); i++) counter[i] = 0;
    }
    
    public void setSquare(Square s1, Square s2) {
        for (int i = 0; i < 4; i++) s1.setSide(i, s2.getSide(i));
        s1.setRotation(s2.getRotation());
        s1.setIndex(s2.getIndex());
    }
    
    public boolean match(Square s1, Square s2, int idx, boolean rotate) {
        matchCounter++;
        if (rotate) {
            for (int i = 0; i < 4; i++) {
                s2.setRotation(i);
                
                if ((s1.getRotatedSide(idx) + 
                        s2.getRotatedSide(getOppositeSide(idx))) == 0) 
                    return true;
            }
        } else {
            if ((s1.getRotatedSide(idx) + 
                        s2.getRotatedSide(getOppositeSide(idx))) == 0) 
                    return true;
        }
        return false;
    }
    
    public boolean match(Square main, Square s1, Square s2, int c, int d) {
        boolean ret = false;
        if (match(s1, main, c, true))
            ret = match(s2, main, d, false);
        if (!ret && match(s2, main, d, true))
            ret = match(s1, main, c, false);
        return ret;
    }
   
    static public int getOppositeSide(int s) {
        int side = s - 2;
        while (side < 0)
            side += 4;
        while (side > 3)
            side -= 4;
        
        return side;
    }
    
    public void calculate() {
        int idx = step;
        int piece = 0, type = 0;
        switch (idx) {
            case 0:
                type = -1;
                break;
            case 1:
            case 2:
                type = 0;
                piece = idx - 1;
                break;
            case 3: 
            case 6:
                type = 1;
                piece = idx - 3;
                break;
            case 4:
            case 5:
            case 7:
            case 8:
                type = 2;
                piece = idx - 3;
                break;
        }
        
        switch (type) {
            case -1:
                if (counter[idx] > 32) step = -1;
                used.clear();
                completed.erase();

                board.getSquare(counter[idx] / 4).setRotation(counter[idx]%4);
                setSquare(completed.getSquare(idx),
                        board.getSquare(counter[idx]/4));
                
                counter[idx]++;
                used.add(counter[idx] / 4);
                step++;
                break;
            case 0:
                for (int i = counter[idx]; i <= board.getSize(); i++) {
                    if (i >= board.getSize()) {
                        used.remove(used.size()-1);
                        completed.erase(idx-1);
                        
                        step--;
                        counter[idx] = 0;
                        completed.getSquare(idx-1).reset();
                        break;
                    } else if (!used.contains(i) &&
                            match(completed.getSquare(idx-1), 
                                  board.getSquare(i), 1, true)) {
                        counter[idx]++;
                        used.add(i);
                        
                        completed.setSquare(idx, board.getSquare(i));
                        
                        step++;
                        break;
                    } else {
                        counter[idx]++;
                        break;
                    }
                }   break;
            case 1:
                for (int i = counter[idx]; i <= board.getSize(); i++) {
                    if (i >= board.getSize()) {
                        used.remove(used.size()-1);
                        completed.erase(idx-1);
                        
                        step--;
                        counter[idx] = 0;
                        break;
                    } else if (!used.contains(i) &&
                            match(completed.getSquare(piece), board.getSquare(i), 2, true)) {
                        counter[idx]++;
                        used.add(i);
                        
                        completed.setSquare(idx, board.getSquare(i));
                        if (idx == board.getSize()-1) step = -1;
                        else step++;
                        
                        break;
                    } else {
                        counter[idx]++;
                        break;
                    }
                }   break;
            case 2:
                for (int i = counter[idx]; i <= board.getSize(); i++) {
                    if (i >= board.getSize()) {
                        used.remove(used.size()-1);
                        completed.erase(idx-1);
                        
                        step--;
                        counter[idx] = 0;
                        break;
                    } else if (!used.contains(i) &&
                            match(board.getSquare(i),
                                    completed.getSquare(piece),
                                    completed.getSquare(piece+2), 2, 1)) {
                        counter[idx]++;
                        used.add(i);
                    
                        setSquare(completed.getSquare(idx), board.getSquare(i));
                        if (idx == board.getSize()-1) step = -1;
                        else step++;
                        
                        break;
                    } else {
                        counter[idx]++;
                        break;
                    }
                }   break;
            default:
                break;
        }
    }
    
    public void animate(Graphics g, int idx) {
        // idx is the current completed square idx
        // 3 means middle left square is showing
        
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                
                if (used.contains(y*3+x)) continue;
                
                board.getSquare(y*3+x).draw(g, 700+220*x, 
                        50+220*y, false);
            }
        }
        
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                if (y*3+x >= idx-1) break;
                
                completed.getSquare(y*3+x).draw(g, 50+200*x, 
                        50+200*y, true);
            }
        }
    }
}

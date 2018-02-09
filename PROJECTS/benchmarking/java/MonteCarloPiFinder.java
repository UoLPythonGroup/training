
import java.util.Random;

class MonteCarloPiFinder implements Runnable {

    int N; //Instance variable number of samples.
    static volatile double pi; //Class variable each instance has access to.
    
    
    MonteCarloPiFinder(int n) {
        N = n;    // Constructor setting number of samples for a particular PiFinder
    }

    @Override
    public void run() {             //Method called when thread containing PiFinder is started.
        double res = getPi();
        pi += res;
        //System.out.println("done: "+res+", total: "+pi);

    }

    public double getPi() {         //MonteCarlo algorithm
        Random r = new Random();
        double x, y, Lpi;
        Lpi = 0;
        for (int i = 0; i < N; i++) {
            x = r.nextDouble();
            y = r.nextDouble();
            if (x * x + y * y <= 1) {
                Lpi += 1;
            }
        }
        return 4.0 * Lpi / N;

    }

    public static void main(String args[]) throws Exception {
        int ThreadNum = Integer.parseInt(args[0]);     //Get thread no. and sample no. from command line
        int SampleNum = Integer.parseInt(args[1]);
        
        long t1 = System.currentTimeMillis();
        Thread[] finders = new Thread[ThreadNum];      //Create thread array 
        for (int i = 0; i < ThreadNum; i++) {                       
            finders[i] = new Thread(new MonteCarloPiFinder(SampleNum));    //Fill it with PiFinders and start them off.
            finders[i].start();
        }
        for (Thread finder : finders) {         //Tell each thread to block the main thread until they are finished.
            finder.join();
        }
        long t2 = System.currentTimeMillis();
        
        System.out.println("Time taken in milliseconds: "+ (t2 - t1));
        System.out.println("Estimate: " + pi / ThreadNum);

        pi = 0;
        //Time doing everything in one thread.
        t1 = System.currentTimeMillis();
        Thread soloFinder = new Thread(new MonteCarloPiFinder(SampleNum * ThreadNum));
        soloFinder.start();
        soloFinder.join();
        t2 = System.currentTimeMillis();
        
        System.out.println("Time taken in milliseconds :"+ (t2 - t1));
        System.out.println("Estimate: " + pi );

    }
}

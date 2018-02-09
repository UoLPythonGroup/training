// Copyright (c) 2014 Bartosz Szczesny <bszcz@bszcz.org>
// This program is free software under the MIT license.

import java.math.BigInteger;

class Fibo {
	public static void main(String[] args) {
		fibo(Integer.parseInt(args[0]));
	}

	private static void fibo(int n) {
		BigInteger old = new BigInteger("0");
		BigInteger now = new BigInteger("1");
		BigInteger sum;
		for (int i = 0; i < (n - 2); ++i) {
			sum = now.add(old);
			old = now;
			now = sum;
		}
		System.out.println(now);
	}
}

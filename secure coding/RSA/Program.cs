using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

//요거 걸어줘야한다. 
using System.Security.Cryptography;


/**
    p = 2
    q = 7

    n = 2 * 7 = 14

    q(n) = (2-1)*(7-1) = 6
    e = 5

    (5*d) mod6 = 1 => d = 6

    공개키 : (14, 5)
    개인키 : (14, 11)

    M = 3 (0<=M<n)
    C = 3^5mod 14 = 5

    컴퓨터로 처리하기 위해서는 큰 수를 다루는 별도의 라이브러리나, 전용 프로그래밍 언어가 필요하다. 일반적인 프로그래밍 언어로는 64비트 정수형 (약 20자리) 보다 큰 수를 직접 제어하는 기능을 제공하지 않는다.
**/
namespace RSA
{
    class Program
    {
        /**
        인자1. 평문
        **/
        static void Main(string[] args)
        {
            int arg_cnt = args.Length;
            if (arg_cnt != 1) {
                Console.WriteLine("평문도 같이 입력해주세요.");
                return;
            }

            int p = 2;
            int q = 7;

            int n, e=0, d=0, f;

            n = p * q;//공개키
            f = (p-1) * (q-1);

            // e 결정
            
            for (int i = 2; i < f; i++) {
                // 2부터 f까지 중에 f와 서로소인 것을 공개키로 정의
                if (gcd(i, f) == 1) {
                    e = i;
                    break;
                }
            }
            
            //d 결정
            
            for (int i = 1; i < n; i++) {
                if ((1 + f * i) % e == 0) {
                    d = (1 + f * i) / e; //개인키
                    break;
                }
            }

            
            int msg = Convert.ToInt32(args[0].Trim());
            ulong C, M;
            Console.WriteLine(msg);
            C = RSA_encypt(msg, e, n);
            Console.WriteLine("암호화:"+C);
            M = RSA_decrypt(C, d, n);
            Console.WriteLine("복호화:"+M);
        
        }



        // 최대 공약수
        public static int gcd(int a, int b) {
            int tmp, n;

            if (a<b) {
                tmp = a;
                a = b;
                b = tmp;
            }

            while (b != 0) {
                n = a % b;
                a = b;
                b = n;
            }

            return a;
        }

       
        
        /*
        rsa 암호화
        msg : 메시지
        e : 공개키
        n : 참조
        */
        public static ulong RSA_encypt(int msg, int e, int n)
        {
            //메시지를 공개키로 거듭제곱한다
            ulong result = (ulong)Math.Pow(msg, e);
            Console.WriteLine("e:"+e);Console.WriteLine("n:"+n);
            Console.WriteLine("제곱은:"+result);
            return result % (ulong)n;
        }

        /*
        rsa 복호화
        msg : 메시지
        d : 개인키
        n : 참조
        */
        public static ulong RSA_decrypt(ulong msg, int d, int n)
        {
            //메시지를 공개키로 거듭제곱한다
            ulong result = (ulong)Math.Pow(msg, d);
            Console.WriteLine("d:"+d);Console.WriteLine("제곱 : "+result);
            return result % (ulong)n;
        }
    }
}


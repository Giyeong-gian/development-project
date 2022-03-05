using System.Text;

//요거 걸어줘야한다. 
using System.Security.Cryptography;

namespace DES
{
    class Program
    {
        //초기 전치 테이블 : 평문 64비트에서 자리를 혼합한다. ex)58비트를 1비트로, 50비트를 2비트로...
        static int[] ip_table = {
            58, 50, 42, 34, 26, 18, 10, 02, 
            60, 52, 44, 36, 28, 20, 12, 04, 
            62, 54, 46, 38, 30, 22, 14, 06, 
            64, 56, 48, 40, 32, 24, 16, 08, 
            57, 49, 41, 33, 25, 17, 09, 01, 
            59, 51, 43, 35, 27, 19, 11, 03, 
            61, 53, 45, 37, 29, 21, 13, 05, 
            63, 55, 47, 39, 31, 23, 15, 07
        };

        //역전치 테이블
        static int[] iip_table = {
            40, 08, 48, 16, 56, 24, 64, 32, 
            39, 07, 47, 15, 55, 23, 63, 31, 
            38, 06, 46, 14, 54, 22, 62, 30, 
            37, 05, 45, 13, 53, 21, 61, 29, 
            36, 04, 44, 12, 52, 20, 60, 28, 
            35, 03, 43, 11, 51, 19, 59, 27, 
            34, 02, 42, 10, 50, 18, 58, 26, 
            33, 01, 41, 09, 49, 17, 57, 25
        };

        //키의 초기 전치 테이블 : 8의 배수 제거하여 64비트에서 56비트로 키 변환
        static int[] initial_key_table = {
            57, 49, 41, 33, 25 ,17, 09,
            01, 58, 50, 42, 34, 26, 18, 
            10, 02, 59, 51, 43, 35, 27, 
            19, 11, 03, 60, 52, 44, 36, 
            63, 55, 47, 39, 31, 23, 15, 
            07, 62, 54, 46, 38, 30, 22, 
            14, 06, 61, 53, 45, 37, 29, 
            21, 13, 05, 28, 20, 12, 04
        };

        //키의 축약 전치 테이블 : 테이블에 없는 값을 버려서 48비트로 키 축약
        static int[] key_compression_table = {
            14, 17, 11, 24, 01, 05, 03, 28, 15, 06, 21, 10, 
            23, 19, 12, 04, 26, 08, 16, 07, 27, 20, 13, 02, 
            41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 
            44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
        };

        static int[] shift_schedule = {
            1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2 ,2, 1
        };

        //확장 테이블 : 32비트는 키와 xor하기 위해 48비트로 확장하기 위한 테이블
        static int[] expansion_table = {
            32, 01, 02, 03, 04, 05, 
            04, 05, 06, 07, 08, 09, 
            08, 09, 10, 11, 12, 13, 
            12, 13, 14, 15, 16, 17, 
            16, 17, 18, 19, 20, 21, 
            20, 21, 22, 23, 24, 25, 
            24, 25, 26, 27, 28, 29, 
            28, 29, 30, 31, 32, 01
        };

        //sbox
        static int[,,] s_box = {
            {
                {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7}, 
                {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8}, 
                {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0}, 
                {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
            },
            {
                {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10}, 
                {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5}, 
                {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15}, 
                {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
            },
            {
                {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8}, 
                {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1}, 
                {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7}, 
                {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
            },
            {
                {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15}, 
                {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9}, 
                {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4}, 
                {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
            },
            {
                {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9}, 
                {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6}, 
                {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14}, 
                {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
            },
            {
                {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11}, 
                {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8}, 
                {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6}, 
                {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
            },
            {
                {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1}, 
                {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6}, 
                {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2}, 
                {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
            },
            {
                {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7}, 
                {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2}, 
                {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8}, 
                {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
            }
        };

        static int[] p_box = {
            16, 07, 20, 21, 29, 12, 28, 17, 01, 15, 23, 26, 05, 18, 31, 10, 
            02, 08, 24, 14, 32, 27, 03, 09, 19, 13, 30, 06, 22, 11, 04, 25
        };

        /**
        인자1. 평문
        인자2. 키
        **/
        static void Main(string[] args)
        {

            int arg_cnt = args.Length;
            if (arg_cnt != 2) {
                Console.WriteLine("평문과 키도 같이 입력해주세요.");
                return;
            }

            string plaintext = fromStringToBit(args[0].Trim());
            string key = fromStringToBit(args[1].Trim());
            Console.WriteLine("암호 전 평문 : "+fromBitToString(plaintext));
            Console.WriteLine("안호 전 평문(2진수) : "+plaintext);
            string encryptedtext = string.Empty;

            Console.WriteLine("-암호화 시작-");

            encryptedtext = des_round(plaintext, key);

            Console.WriteLine("암호문 : "+fromBitToString(encryptedtext));
            Console.WriteLine("암호문(2진수) : "+encryptedtext);

            Console.WriteLine("-복호화 시작-");
            plaintext = decoder(encryptedtext, key);
            
            Console.WriteLine("복호화 : "+fromBitToString(plaintext));
            Console.WriteLine("복호화(2진수) : "+plaintext);
        }

        //문자열을 2진수 문자열로 변환
        public static string fromStringToBit(string plaintext)
        {
            byte[] bytes = UnicodeEncoding.Unicode.GetBytes(plaintext);
            string binaryString = string.Empty; 
            string s = string.Empty;

            foreach (byte b in bytes)
            {
                // byte를 2진수 문자열로 변경
                s = Convert.ToString(b, 2);                
                binaryString += s.PadLeft(8, '0');
            }

            if (binaryString.Length > 64) {
                Console.WriteLine("64비트까지만 입력해주세요.");
                Environment.Exit(1);
            }

            return binaryString.PadLeft(64, '0');
        }

        //2진수 문자열을 문자열로 변환
        public static string fromBitToString(string binString)
        {
            int nbytes = binString.Length / 8;
            byte[] outBytes = new byte[nbytes];

            for (int i = 0; i < nbytes; i++)
            {
                // 8자리 숫자 즉 1바이트 문자열 얻기
                string binStr = binString.Substring(i * 8, 8);
                // 2진수 문자열을 숫자로 변경
                outBytes[i] = (byte)Convert.ToInt32(binStr, 2);
            }

            // Unicode 인코딩으로 바이트를 문자열로
            return UnicodeEncoding.Unicode.GetString(outBytes);
        }

        //전치 함수
        public static string displacement(string bitString, int[] table)
        {
            string result = string.Empty;

            for(int i=0; i < table.Length; i++)
            {
                result += bitString[table[i]-1];
            }

            return result;
        }

        //des 반복 연산 함수
        public static string des_round(string plaintext, string key)
        {
            plaintext = displacement(plaintext, ip_table);
            string left = plaintext.Substring(0,32);
            string right = plaintext.Substring(32);

            //키 초기 전치
            key = displacement(key, initial_key_table);
            string leftKey = key.Substring(0, 28);
            string rightKey = key.Substring(28);
            string subkey = string.Empty;
            string tempRight = string.Empty;

            for(int round=0;round<16;round++) {
                //각 라운드마다 서브키 생성
                leftKey = leftshit(leftKey, shift_schedule[round]);
                rightKey = leftshit(rightKey, shift_schedule[round]);
                subkey =  displacement(leftKey+rightKey, key_compression_table);

                tempRight = feistel(right, subkey);
                tempRight = xor(tempRight, left);

                left = right;
                right = tempRight;
            }
            return displacement(right+left, iip_table);
        }

        public static string decoder(string encryptedtext, string key)
        {
            string plaintext = string.Empty;

            encryptedtext = displacement(encryptedtext, ip_table);

            string left = encryptedtext.Substring(0,32);
            string right = encryptedtext.Substring(32);

            //키 초기 전치
            key = displacement(key, initial_key_table);
            string leftKey = key.Substring(0, 28);
            string rightKey = key.Substring(28);
           
            string[] subkey = new string[16];
            string tempRight = string.Empty;

            for(int round=0;round<16;round++)
            {
                //각 라운드마다 서브키 생성
                leftKey = leftshit(leftKey, shift_schedule[round]);
                rightKey = leftshit(rightKey, shift_schedule[round]);
                subkey[round] =  displacement(leftKey+rightKey, key_compression_table);
            }

            for(int round=15;round>=0;round--)
            {
                tempRight = feistel(right, subkey[round]);
                tempRight = xor(tempRight, left);
                

                left = right;
                right = tempRight;
            }
            
            return displacement(right+left, iip_table);
        }

        public static string leftshit(string bitString, int shift_cnt)
        {
            return bitString.Substring(shift_cnt)+bitString.Substring(0,shift_cnt);
        }

        public static string feistel(string right, string key)
        {
            right = displacement(right, expansion_table);
            right = xor(right, key);
            right = sbox(right);
            right = displacement(right, p_box);
        
            return  right;
        }

        public static string xor(string right, string key)
        {
            string result = string.Empty;

            for(int i=0;i<right.Length;i++)
            {
                result += right[i]^key[i];
            }

            return result;
        }

        public static string sbox(string bitString)
        {

            string result = string.Empty;
            string bit = string.Empty;
            int temp;
            int row, col;
            
            for(int i=0;i<8;i++)
            {
                bit = bitString.Substring(i*6, 6);
                row = Convert.ToInt32(bit.Substring(0,1)+bit.Substring(5,1), 2);
                col = Convert.ToInt32(bit.Substring(1,4), 2);

                temp = s_box[i,row,col];
                result += Convert.ToString(temp, 2).PadLeft(4,'0');
            }
            
            return result;
        }
        
    }
}


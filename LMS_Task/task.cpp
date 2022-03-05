#include <iostream>
#include <cstring>
using namespace std;

struct student{
    char name[10];
    int score[3];
    int sum;
    int average;
    int max;
    int min;
};


int main() {
    
    struct student student[10];
    struct student temp_student;
    
    for(int i=0;i<5;i++){
        cout << "\n" << i+1 << "번째 학생의 이름을 입력하세요 : "; cin >> student[i].name;
        cout << i+1 << "번째 학생의 수학 점수를 입력하세요 : ";
        cin >> student[i].score[0];
        cout << i+1 << "번째 학생의 국어 점수를 입력하세요 : ";
        cin >> student[i].score[1];
        cout << i+1 << "번째 학생의 영어 점수를 입력하세요 : ";
        cin >> student[i].score[2];

        student[i].sum = 0;
        student[i].average = 0;
        student[i].max = 0;
        student[i].min = 100;
        for (int j = 0; j < 3; j++)
        {
            student[i].sum += student[i].score[j];

            if(student[i].max<student[i].score[j])
                student[i].max = student[i].score[j];
            
            if(student[i].min>student[i].score[j])
                student[i].min = student[i].score[j];
        }
        
        
        student[i].average = student[i].sum/3;
        
    }

    for(int i=0; i<4; i++) 
    {
        for(int j=0; (i+j)<4; j++) 
        {
            if(student[j].sum<student[j+1].sum)
            {
                temp_student = student[j];
                student[j] = student[j+1];
                student[j+1] = temp_student;
            }
        }
    }
    
   
    for (int i = 0; i < 5; i++)
    {
        cout << "----------------------------------\n";
        cout << student[i].name << " 성적표\n";
        cout << "수학 : " << student[i].score[0];
        cout << "\n국어 : " << student[i].score[1];
        cout << "\n영어 : " << student[i].score[2];
        cout << "\n총점 : " << student[i].sum;
        cout << "\n평점 : " << student[i].average;
        cout << "\n최고점 : " << student[i].max;
        cout << "\n최저점 : " << student[i].min;
        cout << "\n----------------------------------\n";
    }
    
    
    cout << "\n";

   return 0;
}
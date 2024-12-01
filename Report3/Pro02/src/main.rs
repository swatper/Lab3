use std :: io;
fn main() {
    //입력 받은 값을 저장할 변수
    let mut userInput = String::new();
    println!("두 형렬의 크기를 입력하세요 (row x col)");
    //값 입력 받기
    io::stdin().read_line(&mut userInput).expect("값을 읽지 못 했습니다.");
    let values: Vec<i32> = userInput
        .trim()             //문자열 앞뒤 공백 제거
        .split_whitespace() //문자열을 공백 기준으로 분할
        .map(|s| s.parse().expect("Please enter a number")) //분할한 값을 i32로 파싱
        .collect(); // 파싱된 i32 값들을 벡터로 저장
    let row = values[0] as usize;
    let col = values[1] as usize;

    //2차원 행렬 생성
    let mut matrix_a = vec![vec![0; col]; row];
    let mut matrix_b = vec![vec![0; col]; row];

    //A 행렬 값 입력
    println!("A 행렬의 값 입력");
    for i in 0..row {
        userInput.clear();
        //값 입력 받기
        io::stdin().read_line(&mut userInput).expect("Failed to read line");
        let values: Vec<i32> = userInput
            .trim()
            .split_whitespace()
            .map(|s| s.parse().expect("Please enter a number"))
            .collect();
        //저장
        for j in 0..col {
            matrix_a[i][j] = values[j];
        }
    }

    //B 행렬 값 입력
    println!("B 행렬의 값 입력");
    for i in 0..row {
        userInput.clear();
        //값 입력 받기
        io::stdin().read_line(&mut userInput).expect("Failed to read line");
        let values: Vec<i32> = userInput
            .trim()
            .split_whitespace()
            .map(|s| s.parse().expect("Please enter a number"))
            .collect();
        //저장
        for j in 0..col {
            matrix_b[i][j] = values[j];
        }
    }

    // 행렬 덧셈 결과 출력
    println!("행렬 더하기 결과");
    for i in 0..row {
        for j in 0..col {
            print!("{} ", matrix_a[i][j] + matrix_b[i][j]);
        }
        println!(); // 줄바꿈
    }
}

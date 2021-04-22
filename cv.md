# Daria Varabei

- Telegram @your_dary
- Instagram @dary.sp_
- Github @darya_varabei
## Summary

Hardworking beginner programming student with lots of passion for discovering information technologies and self-belief. 

## Skills

- Python 
- Swift (SwiftUI, Vapor)
- C++
- SQLite

## Code examples
```
func loadData (){
        
        guard let url = URL(string: "http://localhost:8080/api/medication")
        else{
            print("Invalid URL")
            return
        }
        let request = URLRequest(url: url)
        
        URLSession.shared.dataTask(with: request){ data, response, error in
            if let data = data{
                if let decodedResponse = try? JSONDecoder().decode(MedicationResponse.self, from: data){
                    DispatchQueue.main.async {
                        self.results = decodedResponse.results
                    }
                    return
                }
            }
            print("Unable to load data!")
        }.resume()
    }
```

## Experience

A few projects as university tasks. But the best is yet to come

## Education

- 2019 - present – Belarusian State University of Informatics and Radioelectronics, Faculty of Computer-aided Design
- IT-Academy – UX/UI Designer Advanced
- Hacking with Swift Online courses

## English

B2 Level: strong basis from school and university, lots of self-education. Aiming to integrate English into everyday life as much as possible. 

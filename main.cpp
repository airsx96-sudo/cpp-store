#include "httplib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

struct User {
    string username;
    string password;
    string email;
    string role; 
};

unordered_map<string, User> users;          
unordered_map<string, string> active_sessions; 

string get_index_html(const string& current_user, const string& role) {
    string auth_section = "";
    
    if (current_user == "Guest") {
        auth_section = R"(
            <div class='auth-buttons'>
                <button class='btn-secondary' onclick="document.getElementById('loginModal').style.display='block'">تسجيل الدخول</button>
                <button class='btn-primary' onclick="document.getElementById('registerModal').style.display='block'">إنشاء حساب</button>
            </div>
        )";
    } else {
        auth_section = "<div class='user-welcome'>مرحباً، <b>" + current_user + "</b> (" + (role == "admin" ? "مدير" : "عميل") + ") | <a href='/logout' class='logout-link'>تسجيل الخروج</a></div>";
        if (role == "admin") {
            auth_section += " <a href='/admin-dashboard' class='btn-admin' style='margin-right:10px; text-decoration:none;'>لوحة التحكم</a>";
        }
    }

    return R"(
    <!DOCTYPE html>
    <html lang="ar" dir="rtl">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Rifana Yemen Market | سوق ريفنا اليمني</title>
        <style>
            :root {
                --primary-green: #84C325;
                --dark-brown: #544338;
                --golden-yellow: #E9C431;
                --accent-red: #E51A4C;
                --bg-light: #EFEFEF;
                --text-dark: #2D2D2D;
            }

            body {
                background-color: var(--bg-light);
                color: var(--text-dark);
                font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
                margin: 0;
                padding: 0;
            }

            header {
                background-color: white;
                border-bottom: 4px solid var(--primary-green);
                padding: 15px 5%;
                display: flex;
                align-items: center;
                justify-content: space-between;
                box-shadow: 0 2px 10px rgba(0,0,0,0.05);
            }

            .logo-container {
                display: flex;
                align-items: center;
                gap: 15px;
            }

            .logo-img {
                width: 65px;
                height: auto;
                border-radius: 8px;
            }

            .store-title {
                color: var(--dark-brown);
                margin: 0;
                font-size: 24px;
            }

            .store-title span {
                color: var(--primary-green);
            }

            .hero {
                background: linear-gradient(rgba(84, 67, 56, 0.88), rgba(45, 45, 45, 0.9)), url('/logo.jpg');
                background-size: cover;
                background-position: center;
                color: white;
                text-align: center;
                padding: 80px 20px;
            }

            .hero h2 {
                color: var(--golden-yellow);
                font-size: 36px;
                margin-bottom: 10px;
            }

            .container {
                max-width: 1200px;
                margin: 40px auto;
                padding: 0 20px;
            }

            .products-grid {
                display: grid;
                grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
                gap: 30px;
            }

            .product-card {
                background: white;
                border-radius: 12px;
                overflow: hidden;
                box-shadow: 0 4px 15px rgba(0,0,0,0.05);
                border-top: 4px solid var(--primary-green);
                transition: transform 0.3s;
                text-align: center;
                padding: 20px;
            }

            .product-card:hover {
                transform: translateY(-5px);
            }

            .product-title {
                color: var(--dark-brown);
                font-size: 20px;
                margin: 15px 0 10px 0;
            }

            .product-price {
                color: var(--accent-red);
                font-weight: bold;
                font-size: 18px;
                margin-bottom: 15px;
            }

            .btn-primary {
                background-color: var(--primary-green);
                color: white;
                border: none;
                padding: 10px 25px;
                border-radius: 25px;
                cursor: pointer;
                font-weight: bold;
                transition: 0.3s;
            }

            .btn-primary:hover {
                background-color: var(--dark-brown);
            }

            .btn-secondary {
                background-color: transparent;
                color: var(--dark-brown);
                border: 2px solid var(--dark-brown);
                padding: 8px 20px;
                border-radius: 25px;
                cursor: pointer;
                font-weight: bold;
                margin-left: 10px;
            }

            .btn-admin {
                background-color: var(--accent-red);
                color: white;
                padding: 8px 15px;
                border-radius: 5px;
                font-size: 14px;
            }

            .modal {
                display: none;
                position: fixed;
                top: 0; left: 0; width: 100%; height: 100%;
                background-color: rgba(0,0,0,0.5);
                z-index: 1000;
            }

            .modal-content {
                background-color: white;
                margin: 15% auto;
                padding: 30px;
                width: 350px;
                border-radius: 10px;
                border-top: 5px solid var(--primary-green);
                position: relative;
            }

            .close {
                position: absolute;
                top: 10px; left: 15px;
                font-size: 24px; cursor: pointer;
                color: var(--dark-brown);
            }

            .modal input {
                width: 100%;
                padding: 10px;
                margin: 10px 0;
                border: 1px solid #ccc;
                border-radius: 5px;
                box-sizing: border-box;
            }

            .logout-link { color: var(--accent-red); text-decoration: none; font-weight: bold; }
        </style>
    </head>
    <body>

        <header>
            <div class="logo-container">
                <img src="/logo.jpg" alt="Rifana Logo" class="logo-img">
                <h1 class="store-title">Rifana <span>Yemen Market</span></h1>
            </div>
            )" + auth_section + R"(
        </header>

        <section class="hero">
            <h2>مرحباً بكم في سوق ريفنا اليمني الأصيل</h2>
            <p>المتجر الإلكتروني الأول المستوحى من خيرات ومدرجات اليمن السعيد</p>
        </section>

        <div class="container">
            <h2 style="color: var(--dark-brown); border-right: 4px solid var(--primary-green); padding-right: 10px; margin-bottom: 30px;">منتجاتنا المتميزة</h2>
            <div class="products-grid">
                <div class="product-card">
                    <h3 class="product-title">عقيق يمني كبدي أصيل</h3>
                    <div class="product-price">$150.00</div>
                    <button class="btn-primary">إضافة إلى السلة</button>
                </div>
                <div class="product-card">
                    <h3 class="product-title">بن خولاني درجة أولى (1 كيلو)</h3>
                    <div class="product-price">$45.00</div>
                    <button class="btn-primary">إضافة إلى السلة</button>
                </div>
                <div class="product-card">
                    <h3 class="product-title">عسل سدر دوعني فاخر</h3>
                    <div class="product-price">$120.00</div>
                    <button class="btn-primary">إضافة إلى السلة</button>
                </div>
            </div>
        </div>

        <div id="loginModal" class="modal">
            <div class="modal-content">
                <span class="close" onclick="document.getElementById('loginModal').style.display='none'">&times;</span>
                <h3>تسجيل الدخول</h3>
                <form action="/login" method="POST" enctype="multipart/form-data">
                    <input type="text" name="username" placeholder="اسم المستخدم" required>
                    <input type="password" name="password" placeholder="كلمة المرور" required>
                    <button type="submit" class="btn-primary" style="width:100%;">دخول</button>
                </form>
            </div>
        </div>

        <div id="registerModal" class="modal">
            <div class="modal-content">
                <span class="close" onclick="document.getElementById('registerModal').style.display='none'">&times;</span>
                <h3>إنشاء حساب جديد</h3>
                <form action="/register" method="POST" enctype="multipart/form-data">
                    <input type="text" name="username" placeholder="اسم المستخدم" required>
                    <input type="email" name="email" placeholder="البريد الإلكتروني" required>
                    <input type="password" name="password" placeholder="كلمة المرور" required>
                    <button type="submit" class="btn-primary" style="width:100%;">تأكيد التسجيل</button>
                </form>
            </div>
        </div>

    </body>
    </html>
    )";
}

int main() {
    httplib::Server svr;

    users["admin"] = {"admin", "admin123", "admin@rifana.com", "admin"};

    auto get_session_user = [&](const httplib::Request& req) -> pair<string, string> {
        if (req.has_header("Cookie")) {
            string cookie = req.get_header_value("Cookie");
            if (cookie.find("session_id=") != string::npos) {
                size_t pos = cookie.find("session_id=") + 11;
                string session_id = cookie.substr(pos);
                if(session_id.find(";") != string::npos) {
                    session_id = session_id.substr(0, session_id.find(";"));
                }
                if (active_sessions.find(session_id) != active_sessions.end()) {
                    string uname = active_sessions[session_id];
                    return {uname, users[uname].role};
                }
            }
        }
        return {"Guest", "guest"};
    };

    svr.Get("/", [&](const httplib::Request& req, httplib::Response& res) {
        auto [user, role] = get_session_user(req);
        res.set_content(get_index_html(user, role), "text/html; charset=utf-8");
    });

    svr.Get("/logo.jpg", [](const httplib::Request& req, httplib::Response& res) {
        ifstream file("logo.jpg", ios::binary);
        if (!file.is_open()) {
            res.status = 404;
            res.set_content("Logo missing", "text/plain");
            return;
        }
        stringstream buffer;
        buffer << file.rdbuf();
        res.set_content(buffer.str(), "image/jpeg");
    });

    svr.Post("/register", [](const httplib::Request& req, httplib::Response& res) {
        auto username = req.get_file_value("username").content;
        auto email = req.get_file_value("email").content;
        auto password = req.get_file_value("password").content;

        if (!username.empty() && !password.empty() && users.find(username) == users.end()) {
            users[username] = {username, password, email, "user"};
        }
        res.set_redirect("/");
    });

    svr.Post("/login", [](const httplib::Request& req, httplib::Response& res) {
        auto username = req.get_file_value("username").content;
        auto password = req.get_file_value("password").content;

        if (users.find(username) != users.end() && users[username].password == password) {
            string session_id = "sess_" + username + "_" + to_string(rand() % 1000);
            active_sessions[session_id] = username;
            res.set_header("Set-Cookie", "session_id=" + session_id + "; Path=/; HttpOnly");
        }
        res.set_redirect("/");
    });

    svr.Get("/logout", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Set-Cookie", "session_id=; Path=/; Expires=Thu, 01 Jan 1970 00:00:00 GMT");
        res.set_redirect("/");
    });

    svr.Get("/admin-dashboard", [&](const httplib::Request& req, httplib::Response& res) {
        auto [user, role] = get_session_user(req);
        if (role == "admin") {
            string html = "<html lang='ar' dir='rtl'><head><meta charset='UTF-8'><title>لوحة التحكم</title></head><body style='font-family:sans-serif; padding:5%; background:#EFEFEF;'>";
            html += "<h1 style='color:#544338;'>مرحباً بك في لوحة تحكم إدارة ريفنا، " + user + "!</h1>";
            html += "<p>هنا يمكنك إدارة المنتجات والمبيعات وعرض المستخدمين.</p>";
            html += "<br><a href='/' style='background:#84C325; color:white; padding:10px 20px; text-decoration:none; border-radius:5px; font-weight:bold;'>العودة للمتجر</a></body></html>";
            res.set_content(html, "text/html; charset=utf-8");
        } else {
            res.set_content("<h1>غير مسموح بالدخول! للأدمن فقط</h1><a href='/'>رجوع</a>", "text/html; charset=utf-8");
        }
    });

    std::cout << "[+] Rifana Market Server is live on port 8080..." << std::endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}

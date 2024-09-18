import urllib.parse
import requests
import json
import pyfiglet
from colorama import init, Fore, Back, Style

# Inisialisasi Colorama
init(autoreset=True)

result = pyfiglet.figlet_format('Cats Bot')
print(Style.BRIGHT + Fore.BLUE +result)
print(Style.BRIGHT +Fore.GREEN +"\nScript By : @abiedarmawan\n")

# Fungsi untuk mengekstrak informasi user dari query_string
def get_username_from_query_string(query_string):
    parsed_query = urllib.parse.parse_qs(query_string)  # Mem-parse query_string menjadi dictionary
    user_data = parsed_query.get('user', [None])[0]  # Mengambil nilai dari key 'user'
    
    if user_data:
        user_data_json = json.loads(urllib.parse.unquote(user_data))  # Meng-decode dan mengubah user menjadi JSON
        return user_data_json.get('username', 'Unknown')  # Mengambil username dari JSON
    return 'Unknown'

# Membaca file data.txt dengan beberapa query_string
with open('data.txt', 'r') as file:
    query_strings = [line.strip() for line in file if line.strip()]  # Membaca setiap baris dan menghapus whitespace

# Menampilkan jumlah akun
total_accounts = len(query_strings)
print(Fore.LIGHTYELLOW_EX +f"\nTotal accounts : {total_accounts}")
print("="*50)

total_reward_all_accounts = 0

# Step 1: Ambil semua task_id dari tugas
url_taskscats = "https://cats-backend-cxblew-prod.up.railway.app/tasks/user?group=cats"
url_tasksbitget = "https://cats-backend-cxblew-prod.up.railway.app/tasks/user?group=bitget"
url_balance = "https://cats-backend-cxblew-prod.up.railway.app/user"
for idx, query_string in enumerate(query_strings, start=1):
    username = get_username_from_query_string(query_string)
    print(Fore.LIGHTCYAN_EX +f"\naccount {idx} - {username}")
    print("-"*50)

    headers = {
        "accept": "*/*",
        "accept-language": "en-US,en;q=0.9",
        "authorization": f"tma {query_string}",
        "content-type": "application/json",
        "priority": "u=1, i",
        "sec-ch-ua": "\"Chromium\";v=\"128\", \"Not;A=Brand\";v=\"24\", \"Microsoft Edge\";v=\"128\", \"Microsoft Edge WebView2\";v=\"128\"",
        "sec-ch-ua-mobile": "?0",
        "sec-ch-ua-platform": "\"Windows\"",
        "sec-fetch-dest": "empty",
        "sec-fetch-mode": "cors",
        "sec-fetch-site": "cross-site",
        "Referer": "https://cats-frontend.tgapps.store/",
        "Referrer-Policy": "strict-origin-when-cross-origin"
    }

        # Menyelesaikan tasks
    for url_tasks in [url_taskscats, url_tasksbitget]:
        response = requests.get(url_tasks, headers=headers)
        if response.status_code == 200:
            data = response.json()
            tasks = data.get('tasks', [])
            for task in tasks:
                task_id = task.get('id')
                task_title = task.get('title')
                task_reward = task.get('rewardPoints')
                url_complete = f"https://cats-backend-cxblew-prod.up.railway.app/tasks/{task_id}/complete"
                response_complete = requests.post(url_complete, headers=headers, json={})
                if response_complete.status_code == 200:
                    print(Fore.GREEN + f"{task_title} | reward:{task_reward} | berhasil diselesaikan!")
                else:
                    print(Fore.RED + f"{task_title} | reward:{task_reward} | task belum selesai")
        else:
            print(Fore.RED +f"Error fetching tasks: {response.status_code}")

    response = requests.get(url_balance, headers=headers)
    if response.status_code == 200:
        data = response.json()
        total_rewards = data.get('totalRewards', 0)
        total_reward_all_accounts += total_rewards
        print(Fore.LIGHTBLUE_EX +f"Total Rewards: {total_rewards}")
    else:
        print(Fore.RED +f"Error: {response.status_code}")
        print(response.text)


# Menampilkan total reward dari semua akun
print("="*50)
print(Fore.LIGHTMAGENTA_EX + f"Total Reward dari semua akun: {total_reward_all_accounts} Cats")

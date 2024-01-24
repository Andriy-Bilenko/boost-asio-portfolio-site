from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By
from bs4 import BeautifulSoup
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time
from selenium.common.exceptions import TimeoutException

def wait_for_text_change(driver, element_locator, original_text, timeout=60):
    try:
        WebDriverWait(driver, timeout).until(
            EC.text_to_be_present_in_element(element_locator, original_text)
        )
    except TimeoutException:
        print('Timeout: Text did not change within the specified timeout.')

file_path = 'sessions.log'
search_query = 'ip to look up'
url = 'https://iplocation.com/'
#setup
firefox_options = webdriver.FirefoxOptions()
firefox_options.add_argument('--proxy-server=socks5://127.0.0.1:9050')
firefox_options.set_preference('network.proxy.type', 1)
firefox_options.set_preference('network.proxy.socks', '127.0.0.1')
firefox_options.set_preference('network.proxy.socks_port', 9050)
driver = webdriver.Firefox(options=firefox_options)

with open(file_path, 'r') as file:
    for line in file:
        # Find the position of 'on' and 'to' in the line
        index_on = line.find('on')
        index_to = line.find('at')
        search_query = line[index_on + 6:index_to].strip()
        driver.get(url)
        search_field = driver.find_element(By.NAME, 'ip')  
        search_field.send_keys(search_query)
        find_button = driver.find_element(By.CLASS_NAME, 'btn--green') 
        find_button.click()	
        #wait
        element_locator = (By.CLASS_NAME, 'ip')  
        original_text = driver.find_element(*element_locator).text
        wait_for_text_change(driver, element_locator, original_text)
		#wait for rendering/site update just in case (actual case)
        time.sleep(2)
		
		# parsing response from site
        html_response = driver.page_source
        soup = BeautifulSoup(html_response, 'html.parser')
        table = soup.find('table', class_='result-table')
		# Extract
        table_data = {}
        for row in table.find_all('tr'):
            th = row.find('th')
            td = row.find('td')
            if th and td:
                header = th.text.strip()
                value = td.text.strip()
                table_data[header] = value
		# getting rid of ' (change)'
        first_key = next(iter(table_data.keys()))
        first_value = table_data[first_key]
        if "(change)" in first_value:
            first_value = first_value.replace("(change)", "").strip()
            table_data[first_key] = first_value
		
		#print out
        print("======================")
        for key, value in table_data.items():
            print(f"{key}: {value}")

# Close the browser session
driver.quit()


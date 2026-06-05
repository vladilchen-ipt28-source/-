import streamlit as st
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os
import glob

st.set_page_config(layout="wide", page_title="Аналіз індексів VCI/TCI/VHI України")

# Автоматичне завантаження та об'єднання ВСІХ справжніх файлів з папки vhi_data
@st.cache_data
def load_all_real_data():
    path_to_data = os.path.join("vhi_data", "vhi_id_*.csv")
    all_files = glob.glob(path_to_data)
    
    if not all_files:
        st.error(" Папку 'vhi_data' не знайдено в директорії lab5!")
        return pd.DataFrame()
    
    list_of_dfs = []
    area_mapping = {
        1: "Вінницька", 2: "Волинська", 3: "Дніпропетровська", 4: "Донецька", 5: "Житомирська",
        6: "Закарпатська", 7: "Запорізька", 8: "Івано-Франківська", 9: "Київська", 10: "Кіровоградська",
        11: "Луганська", 12: "Львівська", 13: "Миколаївська", 14: "Одеська", 15: "Полтавська",
        16: "Рівненська", 17: "Сумська", 18: "Тернопільська", 19: "Харківська", 20: "Херсонська",
        21: "Хмельницька", 22: "Черкаська", 23: "Чернівецька", 24: "Чернігівська", 25: "Республіка Крим"
    }
    
    for filename in all_files:
        try:
            file_base = os.path.basename(filename)
            area_id = int(file_base.split('_')[2])
        except:
            area_id = 1
            
        # Читаємо файл, повністю ігноруючи текстові рядки заголовків KPI (skiprows=2)
        df = pd.read_csv(filename, sep=',', header=None, skiprows=2, on_bad_lines='skip')
        
        if len(df.columns) >= 7:
            # Примусово даємо назви колонкам строго за порядком їхнього йдення в таблиці ЛР №2
            df.columns = ['Year', 'Week', 'smn', 'smt', 'VCI', 'TCI', 'VHI'] + list(df.columns[7:])
            
            # Конвертуємо перші два стовпчики в числа, щоб перевірити їхні значення
            df['Year'] = pd.to_numeric(df['Year'], errors='coerce')
            df['Week'] = pd.to_numeric(df['Week'], errors='coerce')
            df = df.dropna(subset=['Year', 'Week'])
            
            # Якщо в колонку 'Year' випадково потрапили тижні (значення не перевищують 52), міняємо їх місцями
            if df['Year'].max() <= 52:
                df['Year'], df['Week'] = df['Week'], df['Year']
            
            df["Year"] = df["Year"].astype(int)
            df["Week"] = df["Week"].astype(int)
            
            # Конвертуємо індекси в числовий формат
            for index_col in ['VCI', 'TCI', 'VHI']:
                df[index_col] = pd.to_numeric(df[index_col], errors='coerce')
                
            df["Area"] = area_mapping.get(area_id, f"Область {area_id}")
            list_of_dfs.append(df)
            
    if not list_of_dfs:
        return pd.DataFrame()
        
    return pd.concat(list_of_dfs, ignore_index=True)

df_raw = load_all_real_data()

if df_raw.empty:
    st.error(" Не вдалося завантажити дані. Перевірте вміст папки vhi_data.")
else:
    # Межі для фільтрів на основі виправлених справжніх даних
    MIN_WEEK, MAX_WEEK = 1, 52
    MIN_YEAR = int(df_raw["Year"].min())
    MAX_YEAR = int(df_raw["Year"].max())
    
    if MIN_YEAR > MAX_YEAR:
        MIN_YEAR, MAX_YEAR = 1982, 2024

    all_areas = sorted(df_raw["Area"].unique())

    # [Вимога методички] Інтерактивні елементи в одній колонці (Sidebar)
    st.sidebar.header("Параметри та Fільтри")

    # Ініціалізація стану для кнопки Reset
    if "index_val" not in st.session_state: st.session_state.index_val = "VHI"
    if "area_val" not in st.session_state: st.session_state.area_val = "Київська" if "Київська" in all_areas else all_areas[0]
    if "weeks_val" not in st.session_state: st.session_state.weeks_val = (MIN_WEEK, MAX_WEEK)
    if "years_val" not in st.session_state: st.session_state.years_val = (MIN_YEAR, MAX_YEAR)
    if "sort_asc" not in st.session_state: st.session_state.sort_asc = False
    if "sort_desc" not in st.session_state: st.session_state.sort_desc = False

    # Елементи керування
    selected_index = st.sidebar.selectbox("Оберіть часовий ряд (індекс):", ["VCI", "TCI", "VHI"], key="index_val")
    selected_area = st.sidebar.selectbox("Оберіть область для аналізу:", all_areas, key="area_val")
    selected_weeks = st.sidebar.slider("Інтервал тижнів:", MIN_WEEK, MAX_WEEK, key="weeks_val")
    selected_years = st.sidebar.slider("Інтервал років:", MIN_YEAR, MAX_YEAR, key="years_val")

    st.sidebar.markdown("**Налаштування сортування:**")
    cb_asc = st.sidebar.checkbox("Сортувати за зростанням значень", key="sort_asc")
    cb_desc = st.sidebar.checkbox("Сортувати за спаданням значень", key="sort_desc")

    if cb_asc and cb_desc:
        st.sidebar.warning("⚠️ Обрано обидва режими сортування! Пріоритет надано сортуванню за зростанням.")

    def reset_filters():
        st.session_state.index_val = "VHI"
        st.session_state.area_val = "Київська" if "Київська" in all_areas else all_areas[0]
        st.session_state.weeks_val = (MIN_WEEK, MAX_WEEK)
        st.session_state.years_val = (MIN_YEAR, MAX_YEAR)
        st.session_state.sort_asc = False
        st.session_state.sort_desc = False

    st.sidebar.button("Скинути всі фільтри (Reset)", on_click=reset_filters)

    # Фільтрація масиву даних
    df_filtered = df_raw[
        (df_raw["Area"] == selected_area) &
        (df_raw["Week"] >= selected_weeks[0]) & (df_raw["Week"] <= selected_weeks[1]) &
        (df_raw["Year"] >= selected_years[0]) & (df_raw["Year"] <= selected_years[1])
    ].copy()

    if cb_asc:
        df_filtered = df_filtered.sort_values(by=selected_index, ascending=True)
    elif cb_desc and not cb_asc:
        df_filtered = df_filtered.sort_values(by=selected_index, ascending=False)

    # ГОЛОВНА ПАНЕЛЬ (розміщується в окремій зоні праворуч)
    st.title("Наука про дані: Веб-додаток аналізу сигналів")
    st.markdown(f"Поточний аналіз для області: **{selected_area}** (роки: {selected_years[0]}-{selected_years[1]})")

    # Створення трьох вкладок
    tab_table, tab_single_plot, tab_compare_plot = st.tabs([
        " Таблиця відфільтрованих даних", 
        " Графік часового ряду області", 
        " Графік порівняння областей України"
    ])

    with tab_table:
        st.subheader("Відфільтрований набір справжніх даних")
        st.dataframe(df_filtered, use_container_width=True)

    with tab_single_plot:
        st.subheader(f"Динаміка індексу {selected_index} для області: {selected_area}")
        if df_filtered.empty:
            st.info("Немає даних для відображення.")
        else:
            df_plot = df_filtered.sort_values(by=["Year", "Week"])
            df_plot["Time"] = df_plot["Year"] + (df_plot["Week"] / 52.0)
            
            fig1, ax1 = plt.subplots(figsize=(10, 4.5))
            ax1.plot(df_plot["Time"], df_plot[selected_index], color="teal", lw=2, label=selected_index)
            if not df_plot[selected_index].dropna().empty:
                ax1.axhline(df_plot[selected_index].mean(), color="red", linestyle="--", alpha=0.7, label="Середнє значення")
            ax1.set_xlabel("Роки")
            ax1.set_ylabel("Значення індексу")
            ax1.grid(True, linestyle=":")
            ax1.legend()
            st.pyplot(fig1)

    with tab_compare_plot:
        st.subheader(f"Порівняльний аналіз середнього значення {selected_index} між областями")
        df_all_areas = df_raw[
            (df_raw["Week"] >= selected_weeks[0]) & (df_raw["Week"] <= selected_weeks[1]) &
            (df_raw["Year"] >= selected_years[0]) & (df_raw["Year"] <= selected_years[1])
        ]
        df_grouped = df_all_areas.groupby("Area")[selected_index].mean().reset_index().sort_values(by=selected_index)
        
        if df_grouped.empty:
            st.info("Немає даних для побудови порівняння.")
        else:
            fig2, ax2 = plt.subplots(figsize=(10, 5))
            colors = ["royalblue" if x == selected_area else "lightgray" for x in df_grouped["Area"]]
            bars = ax2.barh(df_grouped["Area"], df_grouped[selected_index], color=colors, edgecolor="black", height=0.6)
            ax2.bar_label(bars, fmt="%.2f", padding=5)
            ax2.grid(axis="x", linestyle=":")
            st.pyplot(fig2)
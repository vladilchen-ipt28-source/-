import streamlit as st
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

st.set_page_config(layout="wide", page_title="Аналіз індексів VCI/TCI/VHI України")

# Емуляція або завантаження даних із Лабораторної роботи №2
@st.cache_data
def load_data():
    np.random.seed(42)
    areas = ["Вінницька", "Волинська", "Дніпропетровська", "Донецька", "Житомирська", 
             "Закарпатська", "Запорізька", "Івано-Франківська", "Київська", "Кіровоградська"]
    data = []
    for year in range(2000, 2025):
        for week in range(1, 53):
            for area in areas:
                data.append({
                    "Year": year, "Week": week, "Area": area,
                    "VCI": np.random.uniform(10, 90),
                    "TCI": np.random.uniform(15, 85),
                    "VHI": np.random.uniform(12, 88)
                })
    return pd.DataFrame(data)

df_raw = load_data()

st.sidebar.header("Параметри та Фільтри")

# [Пункт 1] Dropdown для індексу
if "index_val" not in st.session_state: st.session_state.index_val = "VHI"
selected_index = st.sidebar.selectbox("Оберіть часовий ряд (індекс):", ["VCI", "TCI", "VHI"], key="index_val")

# [Пункт 2] Dropdown для області
all_areas = sorted(df_raw["Area"].unique())
if "area_val" not in st.session_state: st.session_state.area_val = "Київська"
selected_area = st.sidebar.selectbox("Оберіть область для аналізу:", all_areas, key="area_val")

# [Пункт 3] Слайдер тижнів
MIN_WEEK, MAX_WEEK = int(df_raw["Week"].min()), int(df_raw["Week"].max())
if "weeks_val" not in st.session_state: st.session_state.weeks_val = (MIN_WEEK, MAX_WEEK)
selected_weeks = st.sidebar.slider("Інтервал тижнів:", MIN_WEEK, MAX_WEEK, key="weeks_val")

# [Пункт 4] Слайдер років
MIN_YEAR, MAX_YEAR = int(df_raw["Year"].min()), int(df_raw["Year"].max())
if "years_val" not in st.session_state: st.session_state.years_val = (MIN_YEAR, MAX_YEAR)
selected_years = st.sidebar.slider("Інтервал років:", MIN_YEAR, MAX_YEAR, key="years_val")

# [Пункт 5] Кнопка Reset та функція відновлення стану
def reset_filters():
    st.session_state.index_val = "VHI"
    st.session_state.area_val = "Київська"
    st.session_state.weeks_val = (MIN_WEEK, MAX_WEEK)
    st.session_state.years_val = (MIN_YEAR, MAX_YEAR)
    st.session_state.sort_asc = False
    st.session_state.sort_desc = False

st.sidebar.button("Скинути всі фільтри (Reset)", on_click=reset_filters)

# [Пункт 6] Чекбокси сортування з обробкою конфлікту
st.sidebar.markdown("**Налаштування сортування:**")
if "sort_asc" not in st.session_state: st.session_state.sort_asc = False
if "sort_desc" not in st.session_state: st.session_state.sort_desc = False

cb_asc = st.sidebar.checkbox("Сортувати за зростанням значень", key="sort_asc")
cb_desc = st.sidebar.checkbox("Сортувати за спаданням значень", key="sort_desc")

if cb_asc and cb_desc:
    st.sidebar.warning("⚠️ Обрано обидва режими сортування! Пріоритет надано сортуванню за зростанням.")

# Фільтрація масиву на основі обраних параметрів
df_filtered = df_raw[
    (df_raw["Area"] == selected_area) &
    (df_raw["Week"] >= selected_weeks[0]) & (df_raw["Week"] <= selected_weeks[1]) &
    (df_raw["Year"] >= selected_years[0]) & (df_raw["Year"] <= selected_years[1])
].copy()

if cb_asc:
    df_filtered = df_filtered.sort_values(by=selected_index, ascending=True)
elif cb_desc:
    df_filtered = df_filtered.sort_values(by=selected_index, ascending=False)

# [Пункт 7] Головний макет (інтерактивні елементи вже в Sidebar, результати — в основній панелі)
st.title("Наука про дані: Веб-додаток аналізу сигналів")
st.markdown(f"Поточний аналіз для області: **{selected_area}** (роки: {selected_years[0]}-{selected_years[1]})")

# [Пункт 8] Створення трьох вкладок (tabs)
tab_table, tab_single_plot, tab_compare_plot = st.tabs([
    "📊 Таблиця відфільтрованих даних", 
    "📈 Графік часового ряду області", 
    "🗺️ Графік порівняння областей України"
])

with tab_table:
    st.subheader("Відфільтрований набір даних")
    st.dataframe(df_filtered, use_container_width=True)

# [Пункт 9] Візуалізація часового ряду на Вкладці 2
with tab_single_plot:
    st.subheader(f"Динаміка індексу {selected_index} для області: {selected_area}")
    if df_filtered.empty:
        st.info("Немає даних для відображення.")
    else:
        df_plot = df_filtered.sort_values(by=["Year", "Week"])
        df_plot["Time"] = df_plot["Year"] + (df_plot["Week"] / 52.0)

        fig1, ax1 = plt.subplots(figsize=(10, 4.5))
        ax1.plot(df_plot["Time"], df_plot[selected_index], color="teal", lw=2, label=selected_index)
        ax1.axhline(df_plot[selected_index].mean(), color="red", linestyle="--", alpha=0.7, label="Середнє")
        ax1.set_xlabel("Роки")
        ax1.set_ylabel("Значення")
        ax1.grid(True, linestyle=":")
        ax1.legend()
        st.pyplot(fig1)

# [Пункт 10] Побудова порівняльного графіка областей на Вкладці 3
with tab_compare_plot:
    st.subheader(f"Порівняльний аналіз індексу {selected_index} між областями України")
    df_all_areas = df_raw[
        (df_raw["Week"] >= selected_weeks[0]) & (df_raw["Week"] <= selected_weeks[1]) &
        (df_raw["Year"] >= selected_years[0]) & (df_raw["Year"] <= selected_years[1])
    ]
    df_grouped = df_all_areas.groupby("Area")[selected_index].mean().reset_index().sort_values(by=selected_index)

    if df_grouped.empty:
        st.info("Немає даних.")
    else:
        fig2, ax2 = plt.subplots(figsize=(10, 5))
        colors = ["royalblue" if x == selected_area else "lightgray" for x in df_grouped["Area"]]
        bars = ax2.barh(df_grouped["Area"], df_grouped[selected_index], color=colors, edgecolor="black", height=0.6)
        ax2.bar_label(bars, fmt="%.2f", padding=5)
        ax2.grid(axis="x", linestyle=":")
        st.pyplot(fig2)

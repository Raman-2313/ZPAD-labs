import streamlit as st
import pandas as pd
import plotly.express as px

st.set_page_config(layout="wide", page_title="Засоби підготовки та аналізу даних: Лабораторна №5")

@st.cache_data
def load_data():
    df = pd.read_csv('cleaned_data.csv')
    return df

try:
    df = load_data()
except FileNotFoundError:
    st.error("Помилка: Файл 'cleaned_data.csv' не знайдено. Будь ласка, збережіть дані з ЛР №2.")
    st.stop()

provinces = {
    1: "Вінницька", 2: "Волинська", 3: "Дніпропетровська", 4: "Донецька",
    5: "Житомирська", 6: "Закарпатська", 7: "Запорізька", 8: "Івано-Франківська",
    9: "Київська", 10: "Кіровоградська", 11: "Луганська", 12: "Львівська",
    13: "Миколаївська", 14: "Одеська", 15: "Полтавська", 16: "Рівненська",
    17: "Сумська", 18: "Тернопільська", 19: "Харківська", 20: "Херсонська",
    21: "Хмельницька", 22: "Черкаська", 23: "Чернівецька", 24: "Чернігівська",
    25: "Республіка Крим", 26: "Київ", 27: "Севастополь"
}

min_y, max_y = int(df['year'].min()), int(df['year'].max())

col_sidebar, col_main = st.columns([1, 3])

with col_sidebar:
    st.header("Налаштування")
    
    index_choice = st.selectbox("Оберіть індекс:", ["vci", "tci", "vhi"], key="idx")
    
    p_id = st.selectbox("Оберіть область:", options=list(provinces.keys()), 
                        format_func=lambda x: provinces[x], key="province")
    
    week_range = st.slider("Інтервал тижнів:", 1, 52, (1, 52), key="weeks")
    
    year_range = st.slider("Інтервал років:", min_y, max_y, (min_y, max_y), key="years")
    
    st.subheader("Сортування")
    sort_asc = st.checkbox("За зростанням", key="asc")
    sort_desc = st.checkbox("За спаданням", key="desc")
    
    if sort_asc and sort_desc:
        st.warning("Вибрано обидва типи сортування. Пріоритет: за зростанням.")

    if st.button("Скинути всі фільтри"):
        for key in st.session_state.keys():
            del st.session_state[key]
        st.rerun()

filtered_df = df[
    (df['year'] >= year_range[0]) & (df['year'] <= year_range[1]) &
    (df['week'] >= week_range[0]) & (df['week'] <= week_range[1])
].copy()

province_df = filtered_df[filtered_df['province_id'] == p_id].copy()

if sort_asc:
    province_df = province_df.sort_values(by=index_choice, ascending=True)
elif sort_desc:
    province_df = province_df.sort_values(by=index_choice, ascending=False)

with col_main:
    tab_table, tab_plot, tab_compare = st.tabs(["📋 Таблиця", "📉 Графік області", "📊 Порівняння областей"])
    
    with tab_table:
        st.subheader(f"Відфільтровані дані для області: {provinces[p_id]}")
        st.dataframe(province_df, use_container_width=True)
        
    with tab_plot:
        st.subheader(f"Динаміка {index_choice.upper()} за вказаний період")
        if not province_df.empty:
            province_df['time'] = province_df['year'].astype(str) + "-W" + province_df['week'].astype(str)
            fig1 = px.line(province_df, x='time', y=index_choice, 
                           title=f"Зміна {index_choice} для {provinces[p_id]}")
            st.plotly_chart(fig1, use_container_width=True)
        else:
            st.info("Немає даних для відображення за вказаними фільтрами.")
            
    with tab_compare:
        st.subheader(f"Порівняння {index_choice.upper()} області {provinces[p_id]} з іншими")
        comp_df = filtered_df.groupby(['year', 'province_id'])[index_choice].mean().reset_index()
        comp_df['province_name'] = comp_df['province_id'].map(provinces)
        
        fig2 = px.line(comp_df, x='year', y=index_choice, color='province_name',
                       title=f"Порівняння середньорічного {index_choice}")
        
        st.plotly_chart(fig2, use_container_width=True)
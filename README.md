# **Discrete Vortex Method**

![p1](./fig/head.png)

## **Описание проекта**

Этот проект представляет собой реализацию метода дискретных вихрей для анализа аэродинамики профиля крыла. Программа позволяет:

- **Визуализировать линии тока** для профиля крыла.
- Построить **графики распределения давлений** на верхней и нижней поверхностях.
- Вычислить **подъёмную силу** крыла для различных профилей, используя циркуляцию.

Метод дискретных вихрей — это численный подход, который моделирует обтекание профиля крыла, заменяя непрерывное распределение вихрей системой точечных вихрей, размещённых вдоль профиля. 

## **Основные функции**

### 1. Построение линий тока
- Линии тока визуализируют поток воздуха вокруг крыла и помогают понять особенности обтекания.

### 2. Графики распределения давления
- Программа строит графики изменения давления на верхней и нижней поверхностях профиля, демонстрируя аэродинамические свойства.

### 3. Подъёмная сила
- Расчёт безразмерного коэффициента подъёмной силы $C_L$ проводится через циркуляцию, используя теорему Жуковского:

$$
C_L = \frac{2 \Gamma l}{V S}
$$

  где $V$ — скорость потока, $S$ — площадь крыла, $l$ — длина рассматриваемого сегмента крыла, $\Gamma$ — циркуляция, рассчитываемая по интенсивностям вихрей.

## **Технологии**
- Язык: C++
- Библиотеки:
   - **SFML** для визуализации,
   - **Eigen** для работы с линейной алгеброй.  

## **Запуск**
 - Для того чтобы воспользоваться программой достаточно скачать папку VDM_exe и запустить .exe файл.

## **Примеры и особености**
- Метод отлично работает с симметричными профилями, однако несимметричные вызывают трудности, хотя при некоторых значениях параметров их удается решить, вероятно, это связно с плохой обусловленностью матрицы влияния из-за чего система уравнений решается с большой погрешностью.
- На рисунках представлены графики распределения давлений на верхней и нижней поверхностях разных профилей:

![p3](./fig/wing1.png)

![p1](./fig/wing2.png)

![p2](./fig/wing3.png)



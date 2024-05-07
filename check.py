import numpy as np

if __name__ == "__main__":
    SIZE = 1000

    mat_1 = []
    mat_2 = []
    mat_3 = []
    r_1 = []
    r_2 = []
    r_3 = []
    

    matrix_1 = np.genfromtxt('Matrix_1.txt', dtype=int)
    matrix_2 = np.genfromtxt('Matrix_2.txt', dtype=int)
    multiplication = np.genfromtxt('Matrix_res.txt', dtype=int)


    for i in matrix_1:
        r_1.append(i)
        if len(r_1) == SIZE:
            mat_1.append(r_1.copy())
            r_1.clear()

    for j in matrix_2:
        r_2.append(j)
        if len(r_2) == SIZE:
            mat_2.append(r_2.copy())
            r_2.clear()

    for k in multiplication:
        r_3.append(k)
        if len(r_3) == SIZE:
            mat_3.append(r_3.copy())
            r_3.clear()

    mt_1 = np.matrix(np.array(mat_1), dtype=int)
    mt_2 = np.matrix(np.array(mat_2), dtype=int)
    mt_3 = np.matrix(np.array(mat_3), dtype=int)
    mul = np.array(np.dot(mt_1, mt_2))
    mt_3 = np.array(mt_3)

    with open("python_res.txt", "a") as file:
        line = ""
        if np.array_equal(mul, mt_3):
            line = ' Результат совпал'
        else:
            line = ' Результат не совпал'
        file.write("Размер: " + str(SIZE) + line + "\n")
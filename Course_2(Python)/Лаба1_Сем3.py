# Лаба 1
# Курс 2
# Сем  3
import random
random.seed(5)
    
def init_file_one():
    """
    Пункт (1)
    Функция для инициализация файла с указанным именем от пользователя
    в котором хранятся псевдорандомные координаты двух точек в каждой строке
    кол-во строк и числа в диапазоне задаются пользователем 
    """
    while(True):
        try:
            filepath = "Desktop/" + input( "Введите название файла: \n" ) + ".txt"
            break
        except:
            pass

    while(True):
        try:
            size = int(input( "Введите кол-во строк вашего файла: \n" ))
            break
        except:
            print( "!!!!!! Вводите число без букв !!!!!! \n" )
    
    
    while(True):
        try:
            A = int(input( "Введите НИЖНИЙ диапазон координат точек: \n" ))
            break
        except:
            print( "!!!!!! Вводите число без букв !!!!!!  \n" )
    
    while(True):
        try:
            B = int(input( "Введите ВЕРХНИЙ диапазон координат точек: \n" ))
            break
        except:
            print( "!!!!!! Вводите число без букв !!!!!!  \n" )
            
    def my_rand(A, B):
        """
        нужные нам заданные хар-ми числа
        """
        return round(random.uniform(A, B), 2)

    with open(filepath, 'w') as f:
        for i in range(size):
            [x1, y1, x2, y2] = [my_rand(A, B), my_rand(A, B), my_rand(A, B), my_rand(A, B)]
            f.write( f'{x1} {y1} {x2} {y2}\n' )
        

    
    print( f'\nПуть файла: {filepath}',
           f'\nКол-во строк: {size}'
           f'\nДиапазон точек: от {A} до {B}')
    
    C = {'filename': filepath,
            'size': size,
            'range': [A, B] }
    return C


file_one = init_file_one()
print(file_one)

def out_of_first_file(filename):
    """
    Пункт (2)
    Вывод 1-ого файла в нужном формате 
    """
    with open(filename) as f:
        count = 1
        for line in f:            
            
            """Сообщение об ошибке пропущенного значение без исключения"""
            if len(line[:-1].split(" ")) < 4:
                print( f'Обнаружено пропущенное значение:',
                       f'Строка: {line} \n',
                       f'Номер строки: {count}' )
                
            [x1, y1, x2, y2] = line[:-1].split(" ")
            print( f'{count}. {x1}  {y1}     {x2}  {y2}' ) 
            count += 1

out_of_first_file(file_one['filename'])

def init_file_two(file_one, size, filename_two="Desktop/hello2.txt"):
    """
    Пункт (3)
    Инициализация 2-ого файла, зависящего от первого
    """
    
    dots = []
    
    with open(file_one) as f:
        count = 1
        for line in f:
            dots.append(line[:-1].split(" "))
             
    with open(filename_two, 'w') as f:
        for i in dots:
            a = list(map(lambda x: float(x), i))
            [x1, y1, x2, y2] = a
            dist = ( (x1 - x2) ** 2 + (y1 - y2) ** 2 ) ** 0.5
            f.write( f'{dist}\n' )

init_file_two(file_one['filename'], file_one['size'])

def out_file_two(filename_two="Desktop/hello2.txt"):
    """
    Пункт (4)
    Вывод 2-ого файла в консоль
    """
    with open(filename_two) as f:
        for line in f:
            print( line )

out_file_two()

def out_file_two_mean(filename_two="Desktop/hello2.txt"):
    """
    Пункт (6)
    Посчитать среднее 2-ого файла
    """
    summ = 0
    count = 0
    with open(filename_two) as f:
        for line in f:
            summ += float(line)
            count += 1
            
    print( summ/count )

out_file_two_mean()







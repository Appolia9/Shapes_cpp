// Пополнение и использование библиотеки фигур
#include <locale.h>
#include <iostream>
#include "screen.h"
#include "shape.h"
#include <exception>

// ДОБАВЛЕНИЕ креста
class cross : public shape {
private:
	line vertical; // Вертикальная линия
	line horizontal;
	point center;
	int size;// Горизонтальная линия
public:
	cross(point center, int size)
		: vertical(line(point(center.x, center.y - size / 2), point(center.x, center.y + size / 2))),
		horizontal(line(point(center.x - size, center.y), point(center.x + size, center.y))),
		center(center), size(size) {
				if (size <= 0) throw InvalidParamsError();
		if ((!on_screen(center.x, center.y)) || (!on_screen(center.x - size, center.y))
			|| (!on_screen(center.x + size, center.y)) || (!on_screen(center.x, center.y - size / 2 + 1))
			|| (!on_screen(center.x, center.y + size / 2))) throw OutOfScreenError();
	}

	point north() const override { return vertical.north(); }
	point south() const override { return vertical.south(); }
	point east() const override { return horizontal.east(); }
	point west() const override { return horizontal.west(); }
	point neast() const override { return horizontal.neast(); }
	point seast() const override { return horizontal.seast(); }
	point nwest() const override { return vertical.nwest(); }
	point swest() const override { return vertical.swest(); }

	void move(int a, int b) override {
		vertical.move(a, b);
		horizontal.move(a, b);
		center.x += a;
		center.y += b;
	}

	void draw() override {
		vertical.draw();
		horizontal.draw();
	}

	point getCenter_cr() const {
		return center;
	}

	void resize(double d) override {

		if (d <= 0) throw InvalidParamsError();

		size = static_cast<int>(size * d);
		vertical = line(point(center.x, center.y - size / 2), point(center.x, center.y + size / 2));
		horizontal = line(point(center.x - size - 2, center.y), point(center.x + size + 2, center.y));
	}

	void setCenter(point newc) {
		this->center = newc;
		vertical = line(point(center.x, center.y - size / 2), point(center.x, center.y + size / 2));
		horizontal = line(point(center.x - size / 2, center.y), point(center.x + size / 2, center.y));
	}

};


// ПРИМЕР ДОБАВКИ: дополнительный фрагмент – полуокружность
class h_circle : public rectangle, public reflectable {
private:
	int radius;
	point center;
public:
	h_circle(point a, int rd)
		: rectangle(point(a.x - rd, a.y), point(a.x + rd, a.y + rd * 0.7 + 1)), center(a), radius(rd) { }

	void draw();
	void flip_horisontally() { };   // Отразить горизонтально (пустая функция)
	void rotate_right() {}     // Повернуть вправо 
	void rotate_left() {}      // Повернуть влево



	point getCenter_hcr() const {
		return center;
	}

	int getRadius_hcr() const {
		return radius;
	}
	void resize(double d) override {
		// Обновляем радиус
		radius = static_cast<int>(radius * d);

		// Обновляем координаты прямоугольника
		sw = point(center.x - radius, center.y);
		ne = point(center.x + radius, center.y + radius * 0.7 + 1);
	}

};
void h_circle::draw()   //Алгоритм Брезенхэма для окружностей
{  // (выдаются два сектора, указываемые значением reflected::vert)
	int x0 = (sw.x + ne.x) / 2, y0 = vert ? ne.y : sw.y;
	int radius = (ne.x - sw.x) / 2;
	int x = 0, y = radius, delta = 2 - 2 * radius, error = 0;
	while (y >= 0) { // Цикл рисования
		if (vert) { put_point(x0 + x, y0 - y * 0.7); put_point(x0 - x, y0 - y * 0.7); }
		else { put_point(x0 + x, y0 + y * 0.7); put_point(x0 - x, y0 + y * 0.7); }
		error = 2 * (delta + y) - 1;
		if (delta < 0 && error <= 0) { ++x; delta += 2 * x + 1; continue; } //
		error = 2 * (delta - x) - 1;
		if (delta > 0 && error > 0) { --y; delta += 1 - 2 * y; continue; } //
		++x; delta += 2 * (x - y);  --y;
	}
}


class hc_with_cr : public h_circle, public cross {
public:
	hc_with_cr(point center_circle, int radius)
		: h_circle(center_circle, radius),
		cross(point(center_circle.x, center_circle.y + 1), radius) // Центр креста совпадает с центром круга
	{}
	void draw() override {
		h_circle::draw();
		cross::draw();
	}

	void move(int a, int b) override {
		h_circle::move(a, b);
		cross::move(a, b);
	}

	void resize(double d) override {
		h_circle::resize(d);
		cross::resize(d * 0.75);
		updateCrossPosition();
	}


private:
	void updateCrossPosition() {
		// Получаем верхнюю точку круга
		point hc_top = h_circle::north();
		// Перемещаем крест так, чтобы его центр совпадал с верхней точкой круга
		cross::move(hc_top.x - cross::north().x, hc_top.y - cross::north().y);
	}


};

// ПРИМЕР ДОБАВКИ: дополнительная функция присоединения…
void down(shape& p, const shape& q)
{
	point n = q.south();
	point s = p.north();
	p.move(n.x - s.x, n.y - s.y - 1);
	if (!on_screen(p.south().x, p.south().y)) {
		std::cout << "Moving figure down has gone wrong" << std::endl;
		throw OutOfScreenError();
	}
}
// Cборная пользовательская фигура – физиономия
class myshape : public rectangle {   // Моя фигура ЯВЛЯЕТСЯ
	int w, h;			             //        прямоугольником
	line l_eye;    // левый глаз – моя фигура СОДЕРЖИТ линию
	line r_eye;   // правый глаз
	line mouth;  // рот
public:
	myshape(point, point);
	void draw();
	void move(int, int);
	void resize(double r) { rectangle::resize(r); rectangle::move(w * (1 - r) * 0.5, h * (1 - r) * 0.5); }
	void rotate_left() {}
	void rotate_right() {}
};
myshape::myshape(point a, point b)
	: rectangle(a, b),	//Инициализация базового класса
	w(neast().x - swest().x + 1), // Инициализация данных
	h(neast().y - swest().y + 1), // – строго в порядке объявления!
	l_eye(point(swest().x + 2, swest().y + h * 3 / 4), 2),
	r_eye(point(swest().x + w - 4, swest().y + h * 3 / 4), 2),
	mouth(point(swest().x + 2, swest().y + h / 4), w - 4)
{ }
void myshape::draw()
{
	rectangle::draw();      //Контур лица (глаза и нос рисуются сами!) 
	int a = (swest().x + neast().x) / 2;
	int b = (swest().y + neast().y) / 2;
	put_point(point(a, b));   // Нос – существует только на рисунке!
}
void myshape::move(int a, int b)
{
	rectangle::move(a, b);
	l_eye.move(a, b);  r_eye.move(a, b);
	mouth.move(a, b);
}

int main()
{
	setlocale(LC_ALL, "Rus");
	screen_init();
	//h_circle beard(point(60,10), 5);
	try {

		myshape face(point(15, 10), point(27, 18));
		rectangle hat(point(0, 0), point(14, 5));
		h_circle beard(point(40, 10), 5);
		line brim(point(20, 9), 17);
		hc_with_cr tie(60, 5);
		hc_with_cr left_horn(point(60, 10), 5);
		hc_with_cr right_horn(80, 5);
		shape_refresh();
		std::cout << "=== Generated... ===\n";
		std::cin.get(); //Смотреть исходный набор

		//== 2. Подготовка к сборке ==
		hat.rotate_right();
		brim.resize(2.0);
		face.resize(1.2);


		tie.flip_vertically();
		tie.resize(1.2);

		left_horn.flip_vertically();
		left_horn.resize(1.2);

		right_horn.flip_vertically();
		right_horn.resize(1.2);
		shape_refresh();
		std::cout << "=== Prepared... ===\n";
		std::cin.get(); //Смотреть результат поворотов/отражений
		//== 3. Сборка изображения ==
		face.move(20, 0); // Лицо – в исходное положение (если нужно!)
		up(brim, face);
		up(hat, brim);
		down(static_cast<h_circle&>(tie), brim);
		up(static_cast<h_circle&>(left_horn), brim, true);
		up(static_cast<h_circle&>(right_horn), brim, false, true);
		down(static_cast<h_circle&>(tie), face);
		//tie.move(-19,5);
		shape_refresh();
		std::cout << "=== Ready! ===\n";
		std::cin.get();       //Смотреть результат

	}
	catch (const InvalidParamsError& e) {
		std::cout << e.what() << std::endl;
	}
	catch (const OutOfScreenError& e) {
		std::cout << e.what() << std::endl;
	}
	catch (const TransformLimitError& e) {
		std::cout << e.what() << std::endl;
	}
	catch (const My_Error& e) { //const std::exception& e
		std::cout << e.what() << std::endl;
	}
	return 0;
}


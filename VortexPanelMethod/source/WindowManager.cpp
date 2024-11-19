#include "WindowManager.h"

WindowManager::WindowManager() {
    wWidth = 1000;
	wHeight = 800;
	window.create(VideoMode(wWidth, wHeight), L"WingAirForce", Style::Default);
	window.setVerticalSyncEnabled(true);
	size—oeff = 350;

	m = 0;
	p = 0;
	t = 12;
	n = 101;
	vel = 10;
	angle = 0;
	wing.setWingAngle(angle);
	wing.setAirflow(vel, 0);
	wing.setWingPatametrs(n, m, p, t);

    areStreamlines = true;
    isPresure = true;
    isAnglePlot = true;
    isHelpText = true;
	solvedangle = angle;
    startLine = 0; 
    endLine = 2;

    printStartInf();
}
void WindowManager::show() {
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event)){
            controlButtons(event);
        }
        window.clear(Color::Black);
        drawFrame();
        window.display();
    }
}
void WindowManager::controlButtons(Event event) {
    if (event.type == Event::Closed)
        window.close();
    if (event.type == Event::KeyPressed) {
        system("cls");
        if (event.key.code == Keyboard::Z) {
            m -= 1;
            wing.setWingPatametrs(n, m, p, t);
        }
        if (event.key.code == Keyboard::X) {
            m += 1;
            wing.setWingPatametrs(n, m, p, t);
        }
        if (event.key.code == Keyboard::A) {
            p -= 1;
            wing.setWingPatametrs(n, m, p, t);
        }
        if (event.key.code == Keyboard::S) {
            p += 1;
            wing.setWingPatametrs(n, m, p, t);
        }
        if (event.key.code == Keyboard::Q) {
            t -= 1;
            wing.setWingPatametrs(n, m, p, t);
        }
        if (event.key.code == Keyboard::W) {
            t += 1;
            wing.setWingPatametrs(n, m, p, t);
        }
        if (event.key.code == Keyboard::E) {
            n -= 10;
            wing.setWingPatametrs(n, m, p, t);
        }
        if (event.key.code == Keyboard::R) {
            n += 10;
            wing.setWingPatametrs(n, m, p, t);
        }
        if (event.key.code == Keyboard::C) {
            angle -= 0.5;
            wing.setWingAngle(angle);
            wing.setWingPatametrs(n, m, p, t);
        }
        if (event.key.code == Keyboard::V) {
            angle += 0.5;
            wing.setWingAngle(angle);
            wing.setWingPatametrs(n, m, p, t);
        }
        if (event.key.code == Keyboard::Num1) {
            wing.solverType = 1;
        }
        if (event.key.code == Keyboard::Num2) {
            wing.solverType = 2;
        }
        if (event.key.code == Keyboard::Num3) {
            wing.solverType = 3;
        }
        cout << "m" << "\t" << "p" << "\t" << "t" << "\t" << "n" << "\t" << "angle" << endl;
        cout << m << "\t" << p << "\t" << t << "\t" << n << "\t" << angle << endl;
        cout << endl << "Solver: ";
        switch (wing.solverType)
        {
        case 1:
            cout << "GaussianMethod";
            break;
        case 2:
            cout << "LUdecomposition";
            break;
        case 3:
            cout << "QRdecomposition";
            break;
        default:
            cout << " Error ";
            break;
        }
        cout << endl;
        if (event.key.code == Keyboard::Space) {
            wing.solve();
            streamlines.clear();
            pressureUp.clear();
            pressureDown.clear();
            pressureUp = wing.getPressure(true);
            pressureDown = wing.getPressure(false);
            createStreamlines();
            solvedangle = angle;
            anglePlot.push_back(point(solvedangle, wing.Cy));
        }
        if (event.key.code == Keyboard::L) {
            areStreamlines = areStreamlines ? false : true;
        }
        if (event.key.code == Keyboard::P) {
            isPresure = isPresure ? false : true;
        }
        if (event.key.code == Keyboard::O) {
            isAnglePlot = isAnglePlot ? false : true;
        }
        if (event.key.code == Keyboard::H) {
            isHelpText = isHelpText ? false : true;
        }
        if (event.key.code == Keyboard::I) {
            anglePlot.push_back(point(solvedangle, wing.Cy));
        }
        if (event.key.code == Keyboard::J) {
            anglePlot.clear();
        }
        cout << endl;
        cout << "Cy = " << wing.Cy << endl<<endl;
        if (isHelpText) {
            cout << "Control" << endl;
            cout << "m     - Z X" << endl;
            cout << "p     - A S" << endl;
            cout << "t     - Q W" << endl;
            cout << "n     - E R" << endl;
            cout << "angle - C V" << endl;
            cout << "space - solve" << endl;
            cout << "|V|   - mouse left" << endl;
            cout << "change solver - 1 2 3" << endl;
            cout << "linestraem is visible - L" << endl;
            cout << "pressure is visible   - P" << endl;
            cout << "angle plot is visible - O" << endl;
            cout << "delete angle data     - J" << endl;
            cout << "help info is visible  - H" << endl;
        }
    }
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            double x1 = (event.mouseButton.x - 350) / double(size—oeff);
            double y1 = -(event.mouseButton.y - 400) / double(size—oeff);
            std::cout << "|V| = " << wing.getVelocity(point( x1, y1 )).abs() << endl;
        }
    }
}

void WindowManager::drawFrame() {
    drawWing();
    if (areStreamlines)
        drawStreamlines();
    if (isPresure)
        drawPressure();
    if (isAnglePlot)
        drawAnglePlot();
}
void WindowManager::drawWing() {
    CircleShape shape(2.f);
    shape.setFillColor(Color::Yellow);
    for (int i = 0, size = wing.panels.size(); i < size; i++) {
        if (isPresure) {
            if (i < size / 2)
                shape.setFillColor(Color::Blue);
            else
                shape.setFillColor(Color::Red);
        }
        shape.setPosition(size—oeff * wing.panels[i].start.x + 350-2, 400 - size—oeff * wing.panels[i].start.y-2);
        window.draw(shape);
    }
}
void WindowManager::drawStreamlines() {
    if (streamlines.size() != 0) {

        for (int i = 0, size = streamlines.size(); i < size; i++) {
            for (int j = startLine, sizej = endLine - 1; j < sizej; j++) {
                sf::VertexArray line(sf::Lines, 2);
                line[0].position = sf::Vector2f(size—oeff * streamlines[i][j].x + 350, 400 - size—oeff * streamlines[i][j].y);
                line[1].position = sf::Vector2f(size—oeff * streamlines[i][j + 1].x + 350, 400 - size—oeff * streamlines[i][j + 1].y);
                line[0].color = Color(255, 255, 255);
                line[1].color = Color(255, 255, 255);
                window.draw(line);
            }
        }
        if (endLine < streamlines[0].size() / 2) {
            endLine += 1;
        }
        else if (startLine < endLine - 1) {
            startLine += 1;
            if (endLine < streamlines[0].size() - 1)
                endLine += 1;
        }
        else {
            startLine = 0, endLine = 2;
        }
    }
}
void WindowManager::drawPressure() {
    CircleShape shape(2.f);
    shape.setFillColor(Color(0, 0, 255));
    for (int i = 0, size = pressureUp.size(); i < size; i++) {
        shape.setPosition(size—oeff * pressureUp[i].x + 350, 200 - size—oeff / 4 * pressureUp[i].y);
        window.draw(shape);
    }
    shape.setFillColor(Color(255, 0, 0));
    for (int i = 0, size = pressureDown.size(); i < size; i++) {
        shape.setPosition(size—oeff * pressureDown[i].x + 350, 200 - size—oeff / 4 * pressureDown[i].y);
        window.draw(shape);
    }
    sf::VertexArray line(sf::Lines, 2);
    line[0].position = sf::Vector2f(size—oeff * 0 + 350, 200 - size—oeff / 4 * 0);
    line[1].position = sf::Vector2f(size—oeff * 1 + 350, 200 - size—oeff / 4 * 0);
    line[0].color = Color(50, 50, 50);
    line[1].color = Color(50, 50, 50);
    window.draw(line);
    line[0].position = sf::Vector2f(size—oeff * 0 + 350, 200 - size—oeff / 4 * 1);
    line[1].position = sf::Vector2f(size—oeff * 1 + 350, 200 - size—oeff / 4 * 1);
    window.draw(line);

}
void WindowManager::drawAnglePlot() {
    float rad = 3;
    CircleShape shape(rad);
    shape.setRadius(rad);
    shape.setFillColor(Color::White);
    for (int i = 0, size = anglePlot.size(); i < size; i++) {
        shape.setPosition(size—oeff * anglePlot[i].x / 20 + 350 - rad, 650 - size—oeff / 4 * anglePlot[i].y - rad);
        window.draw(shape);
    }
    sf::VertexArray line(sf::Lines, 2);
    line[0].position = sf::Vector2f(size—oeff * 0 + 350, 650 - size—oeff / 4 * 0);
    line[1].position = sf::Vector2f(size—oeff * 1 + 350, 650 - size—oeff / 4 * 0);
    line[0].color = Color(50, 50, 50);
    line[1].color = Color(50, 50, 50);
    window.draw(line);
    line[0].position = sf::Vector2f(size—oeff * 0 + 350, 650 - size—oeff / 4 * 1);
    line[1].position = sf::Vector2f(size—oeff * 1 + 350, 650 - size—oeff / 4 * 1);
    window.draw(line);
    line[0].position = sf::Vector2f(size—oeff * 0 + 350, 650 - size—oeff / 4 * 0);
    line[1].position = sf::Vector2f(size—oeff * 0 + 350, 650 - size—oeff / 4 * 1);
    window.draw(line);
}
void WindowManager::printStartInf() {
    cout << "m" << "\t" << "p" << "\t" << "t" << "\t" << "n" << "\t" << "angle" << endl;
    cout << m << "\t" << p << "\t" << t << "\t" << n << "\t" << angle << endl;
    cout << endl << "Solver: ";
    switch (wing.solverType)
    {
    case 1:
        cout << "GaussianMethod";
        break;
    case 2:
        cout << "LUdecomposition";
        break;
    case 3:
        cout << "QRdecomposition";
        break;
    default:
        cout << " Error ";
        break;
    }
    cout << endl;
    cout << endl << "Cy = " << wing.Cy << endl<<endl;
    if (isHelpText) {
        cout << "Control" << endl;
        cout << "m     - Z X" << endl;
        cout << "p     - A S" << endl;
        cout << "t     - Q W" << endl;
        cout << "n     - E R" << endl;
        cout << "angle - C V" << endl;
        cout << "space - solve" << endl;
        cout << "|V|   - mouse left" << endl;
        cout << "change solver - 1 2 3" << endl;
        cout << "linestraem is visible - L" << endl;
        cout << "pressure is visible   - P" << endl;
        cout << "angle plot is visible - O" << endl;
        cout << "delete angle data     - J" << endl;
        cout << "help info is visible  - H" << endl;
    }
}
void WindowManager::createStreamlines() {
    int nlin = 21;
    double step = 0.02;
    int iter = 4 / step;
    streamlines.resize(nlin);
    for (int i = 0; i < nlin; i++) {
        streamlines[i].push_back(point(-1, 1 - 2 * double(i) / (nlin - 1)));
    }
    for (int i = 1; i < iter; i++) {
        for (int j = 0; j < nlin; j++) {
            point vp = wing.getVelocity(streamlines[j][i - 1]);
            streamlines[j].push_back(streamlines[j][i - 1] + vp / wing.velocity.abs() * step);
        }
    }
}
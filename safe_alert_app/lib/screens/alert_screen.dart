import 'package:flutter/material.dart';

class AlertScreen extends StatelessWidget {
  const AlertScreen({super.key});
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Text(
              'ALERTA',
              style: TextStyle(
                  fontSize: 34,
                  fontWeight: FontWeight.bold,
                  color: Colors.red[800]),
            ),
            SizedBox(height: 20),
            FilledButton(
              onPressed: () {
                Navigator.pop(context);
              },
              child: Text('Llamar a emergencias'),
              style: ButtonStyle(
                backgroundColor: WidgetStateProperty.all(Colors.green[600]),
                padding: WidgetStateProperty.all(EdgeInsets.all(20)),
                shape: WidgetStateProperty.all(RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(10))),
                alignment: Alignment.center,
              ),
            ),
          ],
        ),
      ),
    );
  }
}
